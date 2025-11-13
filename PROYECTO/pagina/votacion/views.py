from django.shortcuts import render, get_object_or_404, redirect
from django.urls import reverse_lazy, reverse
from django.views import generic
from django.utils import timezone
from django.db import transaction, models
from collections import Counter
import json

from django.contrib.auth.decorators import login_required
from django.contrib.auth.mixins import LoginRequiredMixin, UserPassesTestMixin
from django.contrib.auth import login
from django.contrib.auth.forms import UserCreationForm
from django.contrib import messages
from django.contrib.auth.views import LoginView, LogoutView
from django.forms import inlineformset_factory

from .models import Formulario, Question, Choice, EnvioFormulario, Respuesta
from .forms import FormularioForm, QuestionForm

# --- 1. VISTA DE ÍNDICE PÚBLICA (MODIFICADA) ---
class FormularioListView(LoginRequiredMixin, generic.ListView):
    """Muestra los formularios de OTROS usuarios para responder."""
    login_url = '/accounts/login/'
    template_name = 'votacionC/formulario_list_public.html'
    context_object_name = 'formulario_list'

    def get_queryset(self):
        # Mostramos formularios que NO son míos
        return Formulario.objects.filter(
            pub_date__lte=timezone.now(),
            duration__gt=0
        ).exclude(
            propietario=self.request.user
        ).exclude(
            # Excluimos los que ya respondimos
            envios__usuario=self.request.user
        ).order_by('-pub_date')

# --- 2. ¡NUEVA VISTA! EL DASHBOARD DEL USUARIO ---
class MyFormularioListView(LoginRequiredMixin, generic.ListView):
    """Muestra los formularios QUE YO HE CREADO."""
    login_url = '/accounts/login/'
    template_name = 'votacionC/formulario_list_mine.html'
    context_object_name = 'formulario_list'

    def get_queryset(self):
        # Mostramos formularios que SÍ son míos
        return Formulario.objects.filter(
            propietario=self.request.user
        ).order_by('-pub_date')

# --- 3. VISTA DE CREACIÓN DE FORMULARIO ---
@login_required(login_url='/accounts/login/')
def formulario_create_view(request):
    if request.method == 'POST':
        form = FormularioForm(request.POST)
        if form.is_valid():
            formulario = form.save(commit=False)
            formulario.propietario = request.user
            formulario.save()
            messages.success(request, f'¡Formulario "{formulario.titulo}" creado! Ahora añade las preguntas.')
            # Redirigimos al editor
            return redirect('votacion:formulario_edit', pk=formulario.pk)
    else:
        form = FormularioForm()
    return render(request, 'votacionC/formulario_create.html', {'form': form})

# --- 4. VISTA DE EDITOR DE FORMULARIO ---
@login_required(login_url='/accounts/login/')
def formulario_edit_view(request, pk):
    # (El chequeo de propietario ya está aquí, está seguro)
    formulario = get_object_or_404(Formulario, pk=pk, propietario=request.user)
    if request.method == 'POST':
        form = QuestionForm(request.POST)
        if form.is_valid():
            question = form.save(commit=False)
            question.formulario = formulario
            question.save()
            messages.success(request, '¡Pregunta añadida!')
            return redirect('votacion:formulario_edit', pk=formulario.pk)
    else:
        form = QuestionForm()
    questions = formulario.preguntas.all()
    context = {'formulario': formulario, 'questions': questions, 'form': form}
    return render(request, 'votacionC/formulario_edit.html', context)

# --- 5. VISTA DE EDITOR DE OPCIONES ---
@login_required(login_url='/accounts/login/')
def question_edit_options_view(request, pk):
    # (El chequeo de propietario ya está aquí, está seguro)
    question = get_object_or_404(Question, pk=pk, formulario__propietario=request.user)
    if question.tipo_pregunta != Question.QuestionType.MULTIPLE_CHOICE:
        messages.error(request, 'Esta pregunta no es de Opción Múltiple.')
        return redirect('votacion:formulario_edit', pk=question.formulario.pk)
    ChoiceFormSet = inlineformset_factory(Question, Choice, fields=('choice_text', 'orden'), extra=1, can_delete=True, labels={'choice_text': 'Texto de la Opción', 'orden': 'Orden'})
    if request.method == 'POST':
        formset = ChoiceFormSet(request.POST, instance=question)
        if formset.is_valid():
            formset.save()
            messages.success(request, '¡Opciones guardadas con éxito!')
            return redirect('votacion:formulario_edit', pk=question.formulario.pk)
    else:
        formset = ChoiceFormSet(instance=question)
    context = {'question': question, 'formset': formset}
    return render(request, 'votacionC/question_edit.html', context)

# --- 6. VISTA DE RESPONDER FORMULARIO ---
@login_required(login_url='/accounts/login/')
def formulario_respond_view(request, pk):
    formulario = get_object_or_404(Formulario, pk=pk)
    questions = formulario.preguntas.all()
    if formulario.is_closed():
        messages.warning(request, 'Este formulario ya está cerrado.')
        return redirect('votacion:formulario_list')
    if EnvioFormulario.objects.filter(formulario=formulario, usuario=request.user).exists():
        messages.info(request, 'Ya has enviado una respuesta para este formulario.')
        # Redirigimos a la lista pública (o podríamos hacer una vista de resultados pública)
        return redirect('votacion:formulario_list') 
    if request.method == 'POST':
        try:
            with transaction.atomic():
                envio = EnvioFormulario.objects.create(formulario=formulario, usuario=request.user)
                for question in questions:
                    input_name = f'pregunta_{question.pk}'
                    data = request.POST.get(input_name)
                    if data is None: continue
                    respuesta = Respuesta.objects.create(envio=envio, pregunta=question)
                    if question.tipo_pregunta == Question.QuestionType.MULTIPLE_CHOICE:
                        respuesta.opcion_seleccionada_id = int(data)
                    else:
                        respuesta.respuesta_texto = data
                    respuesta.save()
            messages.success(request, '¡Tu respuesta ha sido guardada! Gracias.')
            return redirect('votacion:formulario_list') # Redirigir a la lista pública
        except Exception as e:
            messages.error(request, f'Hubo un error al guardar tu respuesta: {e}')
    context = {'formulario': formulario, 'questions': questions}
    return render(request, 'votacionC/formulario_respond.html', context)

# --- 7. VISTA DE RESULTADOS (PARA EL PROPIETARIO) ---
@login_required(login_url='/accounts/login/')
def formulario_results_view(request, pk):
    formulario = get_object_or_404(Formulario, pk=pk, propietario=request.user)
    questions = formulario.preguntas.all()
    results_data = []
    for question in questions:
        if question.tipo_pregunta == Question.QuestionType.MULTIPLE_CHOICE:
            respuestas = Respuesta.objects.filter(pregunta=question)
            conteo_votos = Counter(respuestas.values_list('opcion_seleccionada_id', flat=True))
            labels = []
            data = []
            for choice in question.opciones.all():
                labels.append(choice.choice_text)
                data.append(conteo_votos.get(choice.pk, 0))
            results_data.append({'question': question, 'type': 'CHOICE', 'labels': json.dumps(labels), 'data': json.dumps(data), 'total_votes': respuestas.count()})
        elif question.tipo_pregunta == Question.QuestionType.OPEN_TEXT:
            respuestas = Respuesta.objects.filter(pregunta=question).values_list('respuesta_texto', flat=True)
            results_data.append({'question': question, 'type': 'TEXT', 'text_responses': list(respuestas)})
    context = {'formulario': formulario, 'results_data': results_data}
    return render(request, 'votacionC/formulario_results.html', context)

# --- 8. VISTA DE BORRADO ---
class FormularioDeleteView(LoginRequiredMixin, UserPassesTestMixin, generic.DeleteView):
    model = Formulario
    template_name = 'votacionC/formulario_confirm_delete.html'
    context_object_name = 'formulario'
    success_url = reverse_lazy('votacion:my_formulario_list') 
    def test_func(self):
        formulario = self.get_object()
        return self.request.user == formulario.propietario
    def delete(self, request, *args, **kwargs):
        messages.success(request, f'El formulario "{self.get_object().titulo}" ha sido eliminado.')
        return super().delete(request, *args, **kwargs)

# --- 9. VISTAS DE AUTENTICACIÓN ---
def register_view(request):
    if request.method == 'POST':
        form = UserCreationForm(request.POST)
        if form.is_valid():
            user = form.save()
            login(request, user)
            messages.success(request, f'¡Bienvenido, {user.username}! Tu cuenta ha sido creada.')
            return redirect('votacion:my_formulario_list') 
    else:
        form = UserCreationForm()
    return render(request, 'registration/register.html', {'form': form})

class CustomLoginView(LoginView):
    template_name = 'registration/login.html'
    def form_valid(self, form):
        messages.success(self.request, f'¡Hola de nuevo, {form.get_user().username}!')
        return super().form_valid(form)

# --- ¡AQUÍ ESTÁ LA CORRECCIÓN! ---
class CustomLogoutView(LogoutView):
    def dispatch(self, request, *args, **kwargs):
        # Usamos 'self.request' en lugar de 'self'
        messages.info(self.request, 'Has cerrado sesión. ¡Vuelve pronto!')
        return super().dispatch(request, *args, **kwargs)
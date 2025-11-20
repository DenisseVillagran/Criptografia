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

import os
from django.conf import settings
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP


# Vista para listar formularios disponibles
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

# Vista para listar mis formularios
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

# Vista para crear un nuevo formulario
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

# Vista para editar un formulario (añadir preguntas)
@login_required(login_url='/accounts/login/')
def formulario_edit_view(request, pk):
    # Verificamos que el usuario sea el propietario
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

# Vista para editar las opciones de una pregunta con opción mul
@login_required(login_url='/accounts/login/')
def question_edit_options_view(request, pk):
    # Verificación del propietario
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

# Vista para responder un formulario
@login_required(login_url='/accounts/login/')
def formulario_respond_view(request, pk):
    formulario = get_object_or_404(Formulario, pk=pk)
    questions = formulario.preguntas.all()

    if formulario.is_closed():
        messages.warning(request, 'Este formulario ya está cerrado.')
        return redirect('votacion:formulario_list')
    
    if EnvioFormulario.objects.filter(formulario=formulario, usuario=request.user).exists():
        messages.info(request, 'Ya has enviado una respuesta para este formulario.')
        return redirect('votacion:formulario_list') 

    if request.method == 'POST':
        try:
            # Recopilación de respuestas en un diccionario
            respuestas_dict = {}
            
            for question in questions:
                input_name = f'pregunta_{question.pk}'
                data = request.POST.get(input_name)
                
                if data:
                    # Guardamos la respuesta usando la pk de la pregunta como clave
                    respuestas_dict[str(question.pk)] = data

            # Cargamos la llave pública
            key_path = os.path.join(settings.BASE_DIR, 'keys', 'public.pem')
            if not os.path.exists(key_path):
                raise FileNotFoundError("No se encontró la llave pública (public.pem) en el servidor")

            public_key = RSA.import_key(open(key_path).read())
            cipher_rsa = PKCS1_OAEP.new(public_key)

            # Pasamos los datos a formato JSON y luego a bytes
            json_data = json.dumps(respuestas_dict)
            bytes_data = json_data.encode('utf-8')

            # Se encriptan los dato
            datos_encriptados = cipher_rsa.encrypt(bytes_data)

            # Se envian los votos encriptados
            # Ya NO creamos objetos Respuesta. Se guardan en un blob cifrado.
            EnvioFormulario.objects.create(
                formulario=formulario, 
                usuario=request.user,
                datos_cifrados=datos_encriptados 
            )

            messages.success(request, 'Tu respuesta ha sido encriptada y guardada con seguridad!')
            return redirect('votacion:formulario_list')

        except Exception as e:
            print(f"Error de encriptación: {e}")
            messages.error(request, f'Hubo un error al procesar tu voto seguro: {e}')

    context = {'formulario': formulario, 'questions': questions}
    return render(request, 'votacionC/formulario_respond.html', context)



# Vista de resultados de formulario
# Aquí se desencriptan los datos
@login_required(login_url='/accounts/login/')
def formulario_results_view(request, pk):
    # Verificamos que sea el propietario (Solo el dueño tiene acceso a la llave privada)
    formulario = get_object_or_404(Formulario, pk=pk, propietario=request.user)
    questions = formulario.preguntas.all()
    
    # Buscamos la llave privada usada para desencriptar
    key_path = os.path.join(settings.BASE_DIR, 'keys', 'private.pem')
    if not os.path.exists(key_path):
        messages.error(request, "Error de Configuración: No se encontró la llave privada del servidor.")
        return redirect('votacion:my_formulario_list')

    try:
        private_key = RSA.import_key(open(key_path).read())
        cipher_rsa = PKCS1_OAEP.new(private_key)
    except Exception as e:
        messages.error(request, f"Error cargando llave privada: {e}")
        return redirect('votacion:my_formulario_list')

    # Obtenemos todos los envíos cifrados del formulario
    envios = EnvioFormulario.objects.filter(formulario=formulario)
    
    # Momento de descifrar todos los votos en memoria
    # Lista de diccionarios: [{'5': 'Opción A', '6': 'Texto...'}, ...]
    votos_descifrados = []
    
    for envio in envios:
        if envio.datos_cifrados:
            try:
                bytes_descifrados = cipher_rsa.decrypt(envio.datos_cifrados)
                json_data = json.loads(bytes_descifrados.decode('utf-8'))
                votos_descifrados.append(json_data)
            except Exception as e:
                print(f"Error descifrando voto {envio.id}: {e}")

    # Ahora si se muestran
    results_data = []

    for question in questions:
        q_id = str(question.pk)
        
        if question.tipo_pregunta == Question.QuestionType.MULTIPLE_CHOICE:
            # Extraer solo las respuestas para ESTA pregunta de todos los votos descifrados
            # Si el voto tiene respuesta para esta pregunta, la tomamos
            respuestas = [voto[q_id] for voto in votos_descifrados if q_id in voto]
            
            try:
                respuestas_ids = [int(r) for r in respuestas]
            except ValueError:
                respuestas_ids = []

            conteo_votos = Counter(respuestas_ids)
            
            labels = []
            data = []
            for choice in question.opciones.all():
                labels.append(choice.choice_text)
                data.append(conteo_votos.get(choice.pk, 0))
            
            results_data.append({
                'question': question, 
                'type': 'CHOICE', 
                'labels': json.dumps(labels), 
                'data': json.dumps(data), 
                'total_votes': len(respuestas_ids)
            })
            
        elif question.tipo_pregunta == Question.QuestionType.OPEN_TEXT:
            respuestas_texto = [voto[q_id] for voto in votos_descifrados if q_id in voto]
            results_data.append({
                'question': question, 
                'type': 'TEXT', 
                'text_responses': respuestas_texto
            })

    context = {'formulario': formulario, 'results_data': results_data}
    return render(request, 'votacionC/formulario_results.html', context)

# Vista de formulario para elminar
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

# Vista de autenticación
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

# Vista de cierre de sesión
class CustomLogoutView(LogoutView):
    def dispatch(self, request, *args, **kwargs):
        # Mensaje para cierre de sesión
        messages.info(self.request, 'Has cerrado sesión. ¡Vuelve pronto!')
        return super().dispatch(request, *args, **kwargs)
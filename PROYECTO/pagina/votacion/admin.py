from django.contrib import admin
from django.db.models import Count
from collections import Counter
import json
from .models import Formulario, Question, Choice, EnvioFormulario, Respuesta

class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3
    min_num = 2

class QuestionInline(admin.StackedInline):
    model = Question
    inlines = [ChoiceInline]
    extra = 1

# Formulario Admin con gráficas 
class FormularioAdmin(admin.ModelAdmin):
    list_display = ('titulo', 'propietario', 'pub_date', 'is_closed', 'conteo_respuestas')
    list_filter = ['pub_date', 'propietario']
    search_fields = ['titulo']
    inlines = [QuestionInline]
    
    # 1. Le decimos que use NUESTRA plantilla personalizada
    change_form_template = 'admin/votacion/formulario/change_form.html'

    def conteo_respuestas(self, obj):
        return obj.envios.count()
    conteo_respuestas.short_description = "Total Respuestas"

    # 2. Sobrescribimos la vista de edición para inyectar datos
    def change_view(self, request, object_id, form_url='', extra_context=None):
        # Obtenemos el objeto formulario
        formulario = self.get_object(request, object_id)
        
        # Si estamos creando uno nuevo (no existe aún), no hacemos nada extra
        if not formulario:
            return super().change_view(request, object_id, form_url, extra_context)

        # Lógica de las gráficas
        results_data = []
        questions = formulario.preguntas.all()

        for question in questions:
            if question.tipo_pregunta == Question.QuestionType.MULTIPLE_CHOICE:
                respuestas = Respuesta.objects.filter(pregunta=question)
                conteo_votos = Counter(respuestas.values_list('opcion_seleccionada_id', flat=True))
                
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
                    'total_votes': respuestas.count()
                })
            elif question.tipo_pregunta == Question.QuestionType.OPEN_TEXT:
                respuestas = Respuesta.objects.filter(pregunta=question).values_list('respuesta_texto', flat=True)
                # Limitamos a las últimas 5 respuestas para no saturar el admin
                results_data.append({
                    'question': question,
                    'type': 'TEXT',
                    'text_responses': list(respuestas)[:5], 
                    'total_count': len(respuestas)
                })

        # Agregamos los datos al contexto
        extra_context = extra_context or {}
        extra_context['results_data'] = results_data
        
        return super().change_view(request, object_id, form_url, extra_context=extra_context)


class RespuestaInline(admin.TabularInline):
    model = Respuesta
    extra = 0 
    can_delete = False 
    fields = ('get_pregunta', 'get_respuesta')
    readonly_fields = ('get_pregunta', 'get_respuesta')

    def get_pregunta(self, obj):
        return obj.pregunta.pregunta_texto
    get_pregunta.short_description = "Pregunta"

    def get_respuesta(self, obj):
        if obj.opcion_seleccionada:
            return obj.opcion_seleccionada.choice_text
        return obj.respuesta_texto
    get_respuesta.short_description = "Respuesta del Usuario"

class EnvioFormularioAdmin(admin.ModelAdmin):
    list_display = ('id', 'formulario', 'usuario', 'fecha_envio')
    list_filter = ('formulario', 'fecha_envio')
    search_fields = ('usuario__username', 'formulario__titulo')
    inlines = [RespuestaInline]
    
    def has_change_permission(self, request, obj=None):
        return False

admin.site.register(Formulario, FormularioAdmin)
admin.site.register(EnvioFormulario, EnvioFormularioAdmin)
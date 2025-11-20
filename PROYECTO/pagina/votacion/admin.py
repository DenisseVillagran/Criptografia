from django.contrib import admin
from django.db.models import Count
from collections import Counter
import json
import os
from django.conf import settings
from django.contrib import messages
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

from .models import Formulario, Question, Choice, EnvioFormulario, Respuesta

# ... (ChoiceInline y QuestionInline se quedan IGUAL) ...
class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3
    min_num = 2

class QuestionInline(admin.StackedInline):
    model = Question
    inlines = [ChoiceInline]
    extra = 1

# --- FORMULARIO ADMIN CON GRÁFICAS ---
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

    # Sobrescribimos la vista para inyectar datos DESCIFRADOS
    def change_view(self, request, object_id, form_url='', extra_context=None):
        # Obtenemos el objeto formulario
        formulario = self.get_object(request, object_id)
        
        # Si estamos creando uno nuevo (no existe aún), no hacemos nada extra
        if not formulario:
            return super().change_view(request, object_id, form_url, extra_context)

        # Intentamos cargar la llave privada
        key_path = os.path.join(settings.BASE_DIR, 'keys', 'private.pem')
        cipher_rsa = None
        
        if os.path.exists(key_path):
            try:
                private_key = RSA.import_key(open(key_path).read())
                cipher_rsa = PKCS1_OAEP.new(private_key)
            except Exception as e:
                self.message_user(request, f"Error cargando llave privada: {e}", level=messages.ERROR)
        else:
            self.message_user(request, "Advertencia: No se encontró 'keys/private.pem'. No se pueden descifrar los resultados.", level=messages.WARNING)

        # Obtenemos envios y desciframos en memoria
        # Solo intentamos descifrar si tenemos el cifrador listo
        votos_descifrados = []
        if cipher_rsa:
            envios = EnvioFormulario.objects.filter(formulario=formulario)
            for envio in envios:
                if envio.datos_cifrados:
                    try:
                        bytes_descifrados = cipher_rsa.decrypt(envio.datos_cifrados)
                        json_data = json.loads(bytes_descifrados.decode('utf-8'))
                        votos_descifrados.append(json_data)
                    except Exception as e:
                        print(f"Admin Error: No se pudo descifrar el envío {envio.id}: {e}")

        # --- LÓGICA DE GRÁFICAS (Igual que en views.py) ---
        results_data = []
        questions = formulario.preguntas.all()

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
                    'text_responses': respuestas_texto[:5], # Solo las últimas 5 para no saturar
                    'total_count': len(respuestas_texto)
                })

        # Para el contexto
        extra_context = extra_context or {}
        extra_context['results_data'] = results_data
        
        return super().change_view(request, object_id, form_url, extra_context=extra_context)

# Como ya no guardamos filas en 'Respuesta', el Inline ya no mostrará nada util

class EnvioFormularioAdmin(admin.ModelAdmin):
    list_display = ('id', 'formulario', 'usuario', 'fecha_envio', 'tiene_datos_cifrados')
    list_filter = ('formulario', 'fecha_envio')
    search_fields = ('usuario__username', 'formulario__titulo')
    
    # Quitamos el inline de respuestas porque ahora todo está en el blob cifrado
    # inlines = [RespuestaInline] 

    def has_change_permission(self, request, obj=None):
        # Solo lectura para asegurar integridad
        return False
    
    def tiene_datos_cifrados(self, obj):
        return bool(obj.datos_cifrados)
    tiene_datos_cifrados.boolean = True
    tiene_datos_cifrados.short_description = "¿Cifrado?"

admin.site.register(Formulario, FormularioAdmin)
admin.site.register(EnvioFormulario, EnvioFormularioAdmin)
admin.site.register(Question)
from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
import datetime


# Este es el nuevo modelo principal que agrupa las preguntas.
class Formulario(models.Model):
    propietario = models.ForeignKey(User, on_delete=models.CASCADE, related_name='formularios')
    titulo = models.CharField(max_length=255)
    descripcion = models.TextField(blank=True, null=True)
    
    # La lógica de tiempo ahora vive aquí, en el formulario principal
    pub_date = models.DateTimeField('fecha de publicación', default=timezone.now)
    duration = models.IntegerField(default=7, help_text="Duración del formulario en días.")

    def __str__(self):
        return self.titulo

    def is_closed(self):
        """Calcula si el formulario está cerrado."""
        closing_date = self.pub_date + datetime.timedelta(days=self.duration)
        return timezone.now() > closing_date

# Este modelo representa una pregunta dentro de un formulario
class Question(models.Model):
    
    # Definimos los tipos de pregunta que permitiremos
    class QuestionType(models.TextChoices):
        MULTIPLE_CHOICE = 'CHOICE', 'Opción Múltiple'
        OPEN_TEXT = 'TEXT', 'Respuesta Abierta'

    formulario = models.ForeignKey(Formulario, on_delete=models.CASCADE, related_name='preguntas')
    pregunta_texto = models.CharField('texto de la pregunta', max_length=500)
    

    tipo_pregunta = models.CharField(
        max_length=10,
        choices=QuestionType.choices,
        default=QuestionType.MULTIPLE_CHOICE
    )
    
    # Campo para ordenar las preguntas
    orden = models.PositiveIntegerField(default=0)

    class Meta:
        ordering = ['orden'] # Ordenar preguntas por este campo

    def __str__(self):
        return f"({self.get_tipo_pregunta_display()}) {self.pregunta_texto[:50]}..."

# Modelo para representar las opciones de una preguta de opción mul
class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE, related_name='opciones')
    choice_text = models.CharField('texto de la opción', max_length=200)
    
    # Campo para ordenar las opciones
    orden = models.PositiveIntegerField(default=0)

    class Meta:
        ordering = ['orden'] # Ordenar opciones por este campo

    def __str__(self):
        return self.choice_text

# Registra el envio de un formulario por un usuario
class EnvioFormulario(models.Model):
    formulario = models.ForeignKey(Formulario, on_delete=models.CASCADE, related_name='envios')
    usuario = models.ForeignKey(User, on_delete=models.CASCADE, related_name='envios')
    fecha_envio = models.DateTimeField(default=timezone.now)

    # --- Cifrado RSA ---
    # Guardamos todas las respuestas del usuario comprimidas en un JSON
    # y luego encriptadas. Al ser RSA, el resultado son BYTES, no texto
    datos_cifrados = models.BinaryField(null=True, blank=True)

    class Meta:
        # Un usuario solo puede enviar un formulario una vez
        unique_together = ('formulario', 'usuario')

    def __str__(self):
        return f"Envío cifrado de {self.usuario.username} para {self.formulario.titulo}"


# Modelo para guardar las respuestas individuales
class Respuesta(models.Model):
    envio = models.ForeignKey(EnvioFormulario, on_delete=models.CASCADE, related_name='respuestas')
    pregunta = models.ForeignKey(Question, on_delete=models.CASCADE, related_name='respuestas')
    
    # Campos de respuesta flexible
    
    # Si es Opción Múltiple, se guarda la FK a la Opción
    opcion_seleccionada = models.ForeignKey(Choice, on_delete=models.SET_NULL, null=True, blank=True)
    
    # Si es Respuesta Abierta, se guarda el texto aquí
    respuesta_texto = models.TextField(null=True, blank=True)

    def __str__(self):
        # Muestra la respuesta correcta dependiendo del tipo
        if self.opcion_seleccionada:
            return f"{self.pregunta.pregunta_texto[:30]}... -> {self.opcion_seleccionada.choice_text}"
        elif self.respuesta_texto:
            return f"{self.pregunta.pregunta_texto[:30]}... -> {self.respuesta_texto[:30]}..."
        else:
            return f"{self.pregunta.pregunta_texto[:30]}... -> (Sin respuesta)"
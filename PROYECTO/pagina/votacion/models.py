from django.db import models
from django.contrib.auth.models import User
from django.utils import timezone
import datetime

# --- 1. El Formulario (El "Abuelo") ---
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

# --- 2. La Pregunta (El "Hijo") ---
# Ahora 'Question' pertenece a un 'Formulario' y tiene un TIPO.
class Question(models.Model):
    
    # Definimos los tipos de pregunta que permitiremos
    class QuestionType(models.TextChoices):
        MULTIPLE_CHOICE = 'CHOICE', 'Opción Múltiple'
        OPEN_TEXT = 'TEXT', 'Respuesta Abierta'
        # (Podrías añadir más aquí en el futuro, como 'CHECKBOX')

    formulario = models.ForeignKey(Formulario, on_delete=models.CASCADE, related_name='preguntas')
    pregunta_texto = models.CharField('texto de la pregunta', max_length=500)
    
    # Este campo es la clave de tu nueva funcionalidad
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

# --- 3. La Opción (El "Nieto") ---
# Este modelo es casi igual, pero 'votes' se elimina.
class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE, related_name='opciones')
    choice_text = models.CharField('texto de la opción', max_length=200)
    
    # 'votes' se elimina porque ya no contamos, almacenamos respuestas.
    
    # Campo para ordenar las opciones
    orden = models.PositiveIntegerField(default=0)

    class Meta:
        ordering = ['orden'] # Ordenar opciones por este campo

    def __str__(self):
        return self.choice_text

# --- 4. El Envío (Quién respondió) ---
# Reemplaza a 'VotoRegistrado'. Registra que un usuario envió un formulario.
class EnvioFormulario(models.Model):
    formulario = models.ForeignKey(Formulario, on_delete=models.CASCADE, related_name='envios')
    usuario = models.ForeignKey(User, on_delete=models.CASCADE, related_name='envios')
    fecha_envio = models.DateTimeField(default=timezone.now)

    class Meta:
        # Un usuario solo puede enviar un formulario una vez
        unique_together = ('formulario', 'usuario')

    def __str__(self):
        return f"Envío de {self.usuario.username} para {self.formulario.titulo}"

# --- 5. La Respuesta (Qué respondió) ---
# Este es el núcleo. Almacena la respuesta a UNA pregunta.
class Respuesta(models.Model):
    envio = models.ForeignKey(EnvioFormulario, on_delete=models.CASCADE, related_name='respuestas')
    pregunta = models.ForeignKey(Question, on_delete=models.CASCADE, related_name='respuestas')
    
    # --- Campos de Respuesta Flexible ---
    
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
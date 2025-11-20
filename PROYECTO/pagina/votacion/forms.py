from django import forms
from .models import Formulario, Question, Choice

class FormularioForm(forms.ModelForm):
    """
    Formulario para crear el objeto 'Formulario' principal.
    (Este ya lo teníamos)
    """
    class Meta:
        model = Formulario
        fields = ['titulo', 'descripcion', 'duration']
        labels = {
            'titulo': 'Título del Formulario',
            'descripcion': 'Descripción (opcional)',
            'duration': 'Duración (en días)'
        }
        widgets = {
            'titulo': forms.TextInput(attrs={'class': 'form-control'}),
            'descripcion': forms.Textarea(attrs={'rows': 3, 'class': 'form-control'}),
            'duration': forms.NumberInput(attrs={'class': 'form-control', 'min': 1, 'value': 7})
        }

# Nuevo formulario para añadir preguntas
class QuestionForm(forms.ModelForm):
    """
    Formulario simple para añadir una nueva pregunta (Question)
    a un Formulario existente.
    """
    class Meta:
        model = Question
        fields = ['pregunta_texto', 'tipo_pregunta']
        labels = {
            'pregunta_texto': 'Texto de la Nueva Pregunta',
            'tipo_pregunta': 'Tipo de Pregunta'
        }
        widgets = {
            'pregunta_texto': forms.TextInput(attrs={'class': 'form-control', 'placeholder': 'Ej: ¿Cuál es tu color favorito?'}),
            'tipo_pregunta': forms.Select(attrs={'class': 'form-select'})
        }
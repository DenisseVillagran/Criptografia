from django.urls import path
from . import views

app_name = 'votacion'

urlpatterns = [
    # Lista PÚBLICA (para responder)
    path('', views.FormularioListView.as_view(), name='formulario_list'),
    
    # Lista PRIVADA (tu dashboard)
    path('mis-formularios/', views.MyFormularioListView.as_view(), name='my_formulario_list'),
    
    # Flujo de creación/edición
    path('crear/', views.formulario_create_view, name='formulario_create'),
    path('<int:pk>/editar/', views.formulario_edit_view, name='formulario_edit'),
    path('pregunta/<int:pk>/editar-opciones/', views.question_edit_options_view, name='question_edit_options'),
    path('<int:pk>/borrar/', views.FormularioDeleteView.as_view(), name='formulario_delete'),
    
    # Flujo de respuesta/resultados
    path('<int:pk>/responder/', views.formulario_respond_view, name='formulario_respond'),
    path('<int:pk>/resultados/', views.formulario_results_view, name='formulario_results'),
]
# pagina/cripto/urls.py
from django.contrib import admin
from django.urls import path, include
from votacion import views as votacion_views
from django.views.generic import RedirectView

urlpatterns = [
    path('', RedirectView.as_view(url='/accounts/login/', permanent=False), name='home'),
    path('admin/', admin.site.urls),
    
    path('votacion/', include('votacion.urls')), 
    
    path('accounts/login/', votacion_views.CustomLoginView.as_view(), name='login'),
    path('accounts/logout/', votacion_views.CustomLogoutView.as_view(next_page='/'), name='logout'),
    path('accounts/register/', votacion_views.register_view, name='register'),
]
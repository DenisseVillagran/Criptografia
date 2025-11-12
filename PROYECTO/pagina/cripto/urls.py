from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path("votacion/", include("votacion.urls")),
    path("admin/", admin.site.urls),
]
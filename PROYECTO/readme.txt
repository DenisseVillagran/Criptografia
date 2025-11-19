INSTRUCCIONES PARA PROBAR EL PROYECTO:

1. Correr el entorno virtual "polling":
source polling/bin/activate

2. Entrar a la direccion de manage.py (cd pagina)
python manage.py runserver

3. Entrar a la direccion
http://127.0.0.1:8000/

http://127.0.0.1:8000/votacion -> lugar donde el usuario vota
http://127.0.0.1:8000/admin -> lugar donde se manejan las preguntas y se recolecta info, se pueden editar cosas,etc.


PARA LA PARTE DE CIFRADO
1. Actualizar migraciones del proyecto (para envio formulario):
> python manage.py makemigrations
> python manage.py migrate

2. Se creó una encuesta llamada 'Prueba2'.
   Para rapido, crear un nuevo usuario y votar en esa encuesta

3. Entrar como admin y comprobar que se siguen actualizando correctamente los votos

(Opcional)
4. Para verificar el cifrado, ejecutar el archivo auditor_db.py
   Este archivo trata de obtener los archivos haciendo una consulta
   Cada que se haga un nuevo voto, se debe ver el contenido del voto cifrado

El archivo generar_llaves se ejecuta una sola vez al inicio, ya no es necesario ejectuarlo
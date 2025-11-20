
# Manual para ejecutar la página

---

## 📌 Requisitos previos

* Linux (Debian/Ubuntu o derivados)
* Python 3
* Git
* Navegador (recomendado: abrir en modo incógnito o borrar caché para evitar errores)

---

## 📥 1. Clonar el repositorio

```bash
git clone <URL_DEL_REPOSITORIO>
cd <NOMBRE_DEL_REPOSITORIO>
cd PROYECTO
```

---

## 🛠️ 2. Crear y activar el entorno virtual

Instalar virtualenv si no lo tienes:

```bash
sudo apt install python3-virtualenv
```

Crear el entorno:

```bash
virtualenv polling
```

Activarlo:

```bash
source polling/bin/activate
```

---

## 📂 3. Entrar a la carpeta del proyecto

```bash
cd pagina/
```

---

## 📦 4. Instalar dependencias

```bash
pip install Django
pip install django-sslserver
pip install pycryptodome
```

---

## 🚀 5. Ejecutar el servidor en HTTPS

```bash
python manage.py runsslserver
```

Al ejecutar, abre tu navegador **en modo incógnito** o **borra el caché**.

La página estará disponible en:

```
https://127.0.0.1:8000
```

---

# English Version

# Manual to run the page


---

## 📌 Requirements

* Linux (Debian/Ubuntu or similar)
* Python 3
* Git
* Browser (open in incognito or clear cache)

---

## 📥 1. Clone the repository

```bash
git clone <URL_OF_REPOSITORY>
cd <REPOSITORY_NAME>
```

---

## 🛠️ 2. Create and activate the virtual environment

Install virtualenv:

```bash
sudo apt install python3-virtualenv
```

Create the environment:

```bash
virtualenv polling
```

Activate it:

```bash
source polling/bin/activate
```

---

## 📂 3. Enter the project folder

```bash
cd pagina/
```

---

## 📦 4. Install dependencies

```bash
pip install Django
pip install django-sslserver
pip install pycryptodome
```

---

## 🚀 5. Start the HTTPS server (loopback)

```bash
python manage.py runsslserver
```

Then open your browser in **incognito mode** or **clear the cache** 

The page will be available at:

```
https://127.0.0.1:8000
```

import sqlite3
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, 'db.sqlite3')

print("--- AUDITOR_DB ---")
print(f"Conectando a la base de datos en: {DB_PATH}")

if not os.path.exists(DB_PATH):
    print("Error, No se encuentra el archivo db.sqlite3 en esa ruta")
    exit()

conn = sqlite3.connect(DB_PATH)
cursor = conn.cursor()

print("Intentando leer la tabla 'votacion_envioformulario'...\n")

try:
    # Ejecutamos consulta
    cursor.execute("SELECT id, datos_cifrados FROM votacion_envioformulario")
    rows = cursor.fetchall()

    if not rows:
        print("La tabla esta vacía...")
    
    for row in rows:
        voto_id = row[0]
        data_raw = row[1]
        
        print(f"[ID Voto: {voto_id}]")
        print(f"CONTENIDO: {data_raw}")
        print(f"TIPO DE DATO: {type(data_raw)}")
        print("-" * 30)

except sqlite3.OperationalError as e:
    print("Error")

finally:
    conn.close()
import sqlite3

# Prueba para ver si se cifraron los datos
# Nos conectan directamente al archivo de base de datos
conn = sqlite3.connect('db.sqlite3')
cursor = conn.cursor()

print("--- AUDITOR_DB ---")
print("Intentando leer la tabla 'votacion_envioformulario'...\n")

try:
    # Ejecutamos consulta
    cursor.execute("SELECT id, datos_cifrados FROM votacion_envioformulario")
    rows = cursor.fetchall()

    if not rows:
        print("La tabla está vacía...")
    
    for row in rows:
        voto_id = row[0]
        data_raw = row[1]
        
        print(f"[ID Voto: {voto_id}]")
        print(f"CONTENIDO: {data_raw}")
        print(f"TIPO DE DATO: {type(data_raw)}")
        print("-" * 30)

except sqlite3.OperationalError:
    print("Error: No se encuentra la tabla.")
finally:
    conn.close()
from Crypto.PublicKey import RSA
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
KEYS_DIR = os.path.join(BASE_DIR, 'keys')

PRIV_FILE = os.path.join(KEYS_DIR, 'private.pem')
PUB_FILE = os.path.join(KEYS_DIR, 'public.pem')
# Verificar si YA existen
if os.path.exists(PRIV_FILE) or os.path.exists(PUB_FILE):
    print(f"Las llaves '{PRIV_FILE}' o '{PUB_FILE}' ya existen")
    print("Cancelando")
    
else:
    # Si no existen, generar nuevas
    print("Generando nuevo par de llaves RSA...")
    key = RSA.generate(2048)

    # Para llave privada
    private_key = key.export_key()
    with open(PRIV_FILE, 'wb') as f:
        f.write(private_key)

    # Para llave pública
    public_key = key.publickey().export_key()
    with open(PUB_FILE, 'wb') as f:
        f.write(public_key)
from Crypto.PublicKey import RSA

print("Generando llaves RSA")
key = RSA.generate(2048)

# Para llave privada
private_key = key.export_key()
with open('private.pem', 'wb') as f:
    f.write(private_key)

# Para llave pública
public_key = key.publickey().export_key()
with open('public.pem', 'wb') as f:
    f.write(public_key)

print("Se crearon 'private.pem' y 'public.pem'")
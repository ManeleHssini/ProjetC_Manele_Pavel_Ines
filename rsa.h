#ifndef RSA_H
#define RSA_H

#include "bigbinary.h"

// Structure pour stocker une clé RSA
typedef struct {
    BigBinary n;   // modulus
    BigBinary e;   // exposant public
    BigBinary d;   // exposant privé
} RSAKey;

// Fonctions publiques
RSAKey RSA_generateKey(BigBinary p, BigBinary q, BigBinary e);
BigBinary RSA_chiffre(BigBinary message, RSAKey key);
BigBinary RSA_dechiffre(BigBinary message, RSAKey key);

// Fonctions internes utiles
BigBinary RSA_phi(BigBinary p, BigBinary q); 
BigBinary RSA_inverseMod(BigBinary a, BigBinary mod);  // Euclide étendu

#endif

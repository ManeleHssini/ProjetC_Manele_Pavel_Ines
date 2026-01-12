
#ifndef BIGBINARY_H
#define BIGBINARY_H

// ====== Structure d'un grand entier binaire ======
typedef struct {
    int *Tdigits;  // Tableau de bits (0 ou 1)
    int Taille;    // Nombre de bits
    int Signe;     // +1 positif, -1 négatif, 0 nul
} BigBinary;

// ====== PHASE 1 : Fonctions de base ======
BigBinary initBigBinaryVide(void);
BigBinary initBigBinaryDepuisChaine(const char *chaine);
void afficheBigBinary(BigBinary nb);
void divisePar2(BigBinary *nb);
void libereBigBinary(BigBinary *nb);

BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B);
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B);

int Egal(const BigBinary *A, const BigBinary *B);
int Inferieur(const BigBinary *A, const BigBinary *B);

// ====== PHASE 2 : Fonctions avancées ======
int estPair(const BigBinary *A);
BigBinary copieBigBinary(const BigBinary *A);
BigBinary multiplyBy2(const BigBinary *A);

BigBinary BigBinary_PGCD(BigBinary A, BigBinary B);
BigBinary BigBinary_mod(BigBinary A, BigBinary B);
BigBinary BigBinary_multiply(const BigBinary *A, const BigBinary *B);
BigBinary BigBinary_multiplyEgypt(const BigBinary *A, const BigBinary *B);
BigBinary BigBinary_expMod(BigBinary M, int exp, BigBinary mod);

// ====== PHASE 3 : RSA avec BigBinary ======
BigBinary BigBinary_expModBigBinary(BigBinary M, BigBinary E, BigBinary N);
BigBinary BigBinary_RSA_Encrypt(BigBinary M, BigBinary E, BigBinary N);
BigBinary BigBinary_RSA_Decrypt(BigBinary C, BigBinary D, BigBinary N);

// Fonctions utilitaires pour RSA
BigBinary intToBigBinary(unsigned int n);
unsigned int bigBinaryToInt(BigBinary bb);

#endif
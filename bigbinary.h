#ifndef BIGBINARY_H
#define BIGBINARY_H

// ====== Représentation d’un grand entier binaire ======
typedef struct {
    int *Tdigits;  // Tableau dynamique contenant les bits (0 ou 1)
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 si positif, -1 si négatif, 0 si nul
} BigBinary;

// ====== Prototypes des fonctions Phase 1 ======
BigBinary initBigBinaryVide(void);
BigBinary initBigBinaryDepuisChaine(const char *chaine);
void      afficheBigBinary(BigBinary nb);
void      divisePar2(BigBinary *nb);
void      libereBigBinary(BigBinary *nb);

BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B);
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B); // suppose A >= B

int Egal(const BigBinary *A, const BigBinary *B);
int Inferieur(const BigBinary *A, const BigBinary *B);

#endif // BIGBINARY_H

// ===== PHASE 2 =====
int estPair(const BigBinary *A);
BigBinary copieBigBinary(const BigBinary *A);
BigBinary multiplyBy2(const BigBinary *A);
BigBinary BigBinary_PGCD(BigBinary A, BigBinary B);
BigBinary BigBinary_mod(BigBinary A, BigBinary B);
BigBinary BigBinary_multiply(const BigBinary *A, const BigBinary *B);
BigBinary BigBinary_multiplyEgypt(const BigBinary *A, const BigBinary *B);
BigBinary BigBinary_expMod(BigBinary M, int exp, BigBinary mod);

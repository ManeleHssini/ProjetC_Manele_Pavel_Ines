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

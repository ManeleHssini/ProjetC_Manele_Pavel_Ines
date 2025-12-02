
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bigbinary.h"



// ======================
// PHASE 1 - UTILITAIRES
// ======================

// ===============================
// 1. Fonction d'initialisation vide
// ===============================
// Crée un BigBinary valant 0
BigBinary initBigBinaryVide(void) {
    BigBinary nb;
    nb.Taille = 1;
    nb.Signe = 0; // 0 = nul
    nb.Tdigits = (int*)malloc(sizeof(int));
    if (!nb.Tdigits) exit(1); // en cas d'erreur mémoire
    nb.Tdigits[0] = 0; // seul bit = 0
    return nb;
}

// ===============================
// 2. Fonction d'initialisation depuis une chaîne binaire
// ===============================
// Exemple d'appel : initBigBinaryDepuisChaine("101001");
BigBinary initBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;

    if (chaine == NULL || *chaine == '\0')
        return initBigBinaryVide();

    int longueur = strlen(chaine);
    nb.Tdigits = (int*)malloc(longueur * sizeof(int));
    if (!nb.Tdigits) exit(1);

    nb.Taille = longueur;
    nb.Signe = +1;

    int allZero = 1;   // <-- on surveille si tous les bits sont 0

    for (int i = 0; i < longueur; i++) {
        nb.Tdigits[i] = (chaine[i] == '1') ? 1 : 0;
        if (nb.Tdigits[i] == 1)
            allZero = 0;
    }

    if (allZero)
        nb.Signe = 0;   // <-- nombre nul

    return nb;
}


// Affiche un BigBinary : signe, puis bits MSB->LSB, ou "0" si nul
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; ++i) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

// Fonction : divisePar2
// But : Divise un grand entier binaire par 2 en décalant les bits vers la droite
void divisePar2(BigBinary *nb) {

    if (nb == NULL || nb->Signe == 0 || nb->Taille == 0)
        return;

    // Nouveau nombre = on enlève le dernier bit (LSB)
    int newSize = nb->Taille - 1;

    if (newSize == 0) {
        // Le nombre devient 0
        nb->Tdigits[0] = 0;
        nb->Taille = 1;
        nb->Signe = 0;
        return;
    }

    // On copie tous les bits sauf le dernier
    int *newArr = (int*)malloc(sizeof(int) * newSize);
    for (int i = 0; i < newSize; i++) {
        newArr[i] = nb->Tdigits[i];
    }

    free(nb->Tdigits);
    nb->Tdigits = newArr;
    nb->Taille = newSize;

    // Vérification si le résultat est 0
    int allZero = 1;
    for (int i = 0; i < nb->Taille; i++)
        if (nb->Tdigits[i] == 1)
            allZero = 0;

    nb->Signe = allZero ? 0 : +1;
}


// ===============================================
// Fonction : libereBigBinary
// But : Libérer la mémoire réservée pour un BigBinary
// ===============================================
void libereBigBinary(BigBinary *nb) {
    if (nb == NULL) return;        // sécurité : si on passe un pointeur vide
    free(nb->Tdigits);             // libère le tableau de bits
    nb->Tdigits = NULL;            // évite les pointeurs “dangereux”
    nb->Taille = 0;                // remet à zéro la taille
    nb->Signe = 0;                 // et le signe
}


// ===============================================
// Fonction : additionBigBinary (algorithme "naïf")
// But      : Calculer R = A + B (avec A,B >= 0)
// Remarque : Tdigits[0] = MSB, Tdigits[Taille-1] = LSB
// ===============================================
BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B) {
    // Longueurs
    int nA = A ? A->Taille : 0;
    int nB = B ? B->Taille : 0;

    // Taille maximale + 1 (pour une éventuelle retenue supplémentaire)
    int n   = (nA > nB ? nA : nB);
    int cap = n + 1;

    // Alloue le résultat avec cap bits initialisés à 0
    BigBinary R;
    R.Taille  = cap;
    R.Signe   = +1;                         // par défaut positif (on ajustera si 0)
    R.Tdigits = (int*)malloc(sizeof(int)*cap);
    if (!R.Tdigits) exit(1);
    for (int i = 0; i < cap; ++i) R.Tdigits[i] = 0;

    // On additionne de DROITE à GAUCHE (LSB -> MSB)
    int iA = nA - 1;        // index sur A (LSB)
    int iB = nB - 1;        // index sur B (LSB)
    int iR = cap - 1;       // index sur R (LSB)
    int carry = 0;          // retenue

    while (iA >= 0 || iB >= 0 || carry) {
        int bitA = (iA >= 0) ? A->Tdigits[iA] : 0;
        int bitB = (iB >= 0) ? B->Tdigits[iB] : 0;
        int s    = bitA + bitB + carry;     // somme binaire + retenue

        R.Tdigits[iR] = (s & 1);            // s % 2
        carry         = (s >> 1);           // s / 2 (0 ou 1 en binaire)

        iA--; iB--; iR--;
    }
    // Les cases restantes à gauche (si iR >= 0) restent à 0

    // ---- Normalisation simple : enlever le 0 de tête s’il n’y a pas de retenue finale ----
    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0) first++;

    if (first > 0) {
        int newLen = R.Taille - first;
        int *newArr = (int*)malloc(sizeof(int)*newLen);
        if (!newArr) exit(1);
        for (int k = 0; k < newLen; ++k) newArr[k] = R.Tdigits[first + k];
        free(R.Tdigits);
        R.Tdigits = newArr;
        R.Taille  = newLen;
    }

    // Si le résultat est 0 (tous bits à 0), on met Signe=0
    int allZero = 1;
    for (int i = 0; i < R.Taille; ++i) if (R.Tdigits[i] == 1) { allZero = 0; break; }
    R.Signe = allZero ? 0 : +1;

    return R;
}

// ===============================================
// Fonction : soustractionBigBinary (algorithme "naïf")
// But      : Calculer R = A - B avec A >= B et A,B >= 0
// Remarque : Tdigits[0] = MSB, Tdigits[Taille-1] = LSB
// ===============================================
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B) {

    BigBinary R;
    R.Taille  = A->Taille;
    R.Signe   = +1;
    R.Tdigits = malloc(R.Taille * sizeof(int));

    int borrow = 0;

    for (int i = 1; i <= A->Taille; i++) {

        int a = A->Tdigits[A->Taille - i];
        int b = (i <= B->Taille) ? B->Tdigits[B->Taille - i] : 0;

        int d = a - borrow - b;

        if (d < 0) {
            d += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }

        R.Tdigits[R.Taille - i] = d;
    }

    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0) first++;

    int newLen = R.Taille - first;
    int *newArr = malloc(newLen * sizeof(int));
    for (int k = 0; k < newLen; k++)
        newArr[k] = R.Tdigits[first + k];

    free(R.Tdigits);

    R.Tdigits = newArr;
    R.Taille  = newLen;
    R.Signe   = (newLen == 1 && newArr[0] == 0) ? 0 : +1;

    return R;
}



// ===============================================
// Fonction : Egal
// But : Vérifie si A et B représentent le même nombre binaire
// ===============================================
int Egal(const BigBinary *A, const BigBinary *B) {
    // Si les signes sont différents → forcément pas égaux
    if (A->Signe != B->Signe) return 0;

    // Si les tailles sont différentes → pas égaux non plus
    if (A->Taille != B->Taille) return 0;

    // On compare bit par bit
    for (int i = 0; i < A->Taille; ++i) {
        if (A->Tdigits[i] != B->Tdigits[i]) return 0;
    }

    // Si on arrive ici → tout est identique
    return 1;
}

// ===============================================
// Fonction : Inferieur
// But : Vérifie si A < B (avec A,B >= 0 dans la Phase 1)
// ===============================================
int Inferieur(const BigBinary *A, const BigBinary *B) {

    if (A->Signe == 0 && B->Signe != 0) return 1;
    if (A->Signe != 0 && B->Signe == 0) return 0;

    if (A->Taille < B->Taille) return 1;
    if (A->Taille > B->Taille) return 0;

    for (int i = 0; i < A->Taille; i++) {
        if (A->Tdigits[i] < B->Tdigits[i]) return 1;
        if (A->Tdigits[i] > B->Tdigits[i]) return 0;
    }
    return 0;
}


// ======================
// PHASE 2 - UTILITAIRES
// ======================

BigBinary copieBigBinary(const BigBinary *A) {
    BigBinary C;
    C.Taille = A->Taille;
    C.Signe  = A->Signe;
    C.Tdigits = (int*)malloc(sizeof(int) * C.Taille);

    for (int i = 0; i < C.Taille; i++)
        C.Tdigits[i] = A->Tdigits[i];

    return C;
}
int estPair(const BigBinary *A) {
    if (A->Signe == 0) return 1; // 0 est pair
    return (A->Tdigits[A->Taille - 1] == 0);
}

BigBinary multiplyBy2(const BigBinary *A) {
    BigBinary R;
    R.Taille = A->Taille + 1;
    R.Signe  = A->Signe;

    R.Tdigits = malloc(sizeof(int) * R.Taille);

    for (int i = 0; i < A->Taille; i++)
        R.Tdigits[i] = A->Tdigits[i];

    R.Tdigits[R.Taille - 1] = 0; // décalage à gauche

    return R;
}


// ======================
// PHASE 2 - PGCD BINAIRE
// ======================


BigBinary BigBinary_PGCD(BigBinary A, BigBinary B) {

    // 1. Cas simples
    if (A.Signe == 0) return copieBigBinary(&B);
    if (B.Signe == 0) return copieBigBinary(&A);

    // Copies locales modifiables
    BigBinary a = copieBigBinary(&A);
    BigBinary b = copieBigBinary(&B);

    int k = 0;

    // 2. Enlever les facteurs 2 communs
    while (estPair(&a) && estPair(&b)) {
        divisePar2(&a);
        divisePar2(&b);
        k++;
    }

    // 3. rendre a impair
    while (estPair(&a)) {
        divisePar2(&a);
    }

    // 4. Boucle principale
    while (b.Signe != 0) {

        while (estPair(&b)) {
            divisePar2(&b);
        }

        if (Inferieur(&b, &a)) {
            BigBinary tmp = a;
            a = b;
            b = tmp;
        }

        BigBinary diff = soustractionBigBinary(&b, &a);
        libereBigBinary(&b);
        b = diff;
    }

    // 5. Reconstruction du PGCD = a × 2^k
    while (k > 0) {
        BigBinary tmp = multiplyBy2(&a);
        libereBigBinary(&a);
        a = tmp;
        k--;
    }

    return a;
}


BigBinary BigBinary_mod(BigBinary A, BigBinary B) {

    // Cas simples
    if (B.Signe == 0) {
        printf("Erreur : modulo par zero.\n");
        return initBigBinaryVide();
    }

    BigBinary R = copieBigBinary(&A);  // on travaille sur une copie

    // Tant que R >= B, on soustrait
    while (!Inferieur(&R, &B)) {
        BigBinary tmp = soustractionBigBinary(&R, &B);
        libereBigBinary(&R);
        R = tmp;
    }

    return R;
}



BigBinary BigBinary_multiply(const BigBinary *A, const BigBinary *B) {

    // Si un des 2 est nul → resultat = 0
    if (A->Signe == 0 || B->Signe == 0)
        return initBigBinaryVide();

    // Résultat max = Taille A + Taille B
    int nA = A->Taille;
    int nB = B->Taille;
    int nR = nA + nB;

    // initialiser à 0
    BigBinary R;
    R.Taille = nR;
    R.Signe = +1;
    R.Tdigits = calloc(nR, sizeof(int));

    for (int iB = nB - 1; iB >= 0; iB--) {
        if (B->Tdigits[iB] == 1) {

            // Ajouter A décalé à gauche (shift)
            int shift = (nB - 1 - iB);

            for (int iA = nA - 1; iA >= 0; iA--) {
                int posR = (nR - 1) - (nA - 1 - iA) - shift;
                R.Tdigits[posR] += A->Tdigits[iA];
            }
        }
    }

    // gérer les retenues
    for (int i = nR - 1; i > 0; i--) {
        if (R.Tdigits[i] > 1) {
            R.Tdigits[i - 1] += R.Tdigits[i] / 2;
            R.Tdigits[i] %= 2;
        }
    }

    // normalisation
    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0)
        first++;

    if (first > 0) {
        int newLen = R.Taille - first;
        int *newArr = malloc(newLen * sizeof(int));
        for (int k = 0; k < newLen; k++)
            newArr[k] = R.Tdigits[first + k];

        free(R.Tdigits);
        R.Tdigits = newArr;
        R.Taille = newLen;
    }

    return R;
}


BigBinary BigBinary_expMod(BigBinary M, int exp, BigBinary mod) {

    // result = 1
    BigBinary result = initBigBinaryDepuisChaine("1");

    // M = M % mod
    BigBinary Mmod = BigBinary_mod(M, mod);

    while (exp > 0) {

        // si exp impair
        if (exp & 1) {
            BigBinary tmp = BigBinary_multiplyEgypt(&result, &Mmod);
            BigBinary tmp2 = BigBinary_mod(tmp, mod);

            libereBigBinary(&result);
            libereBigBinary(&tmp);

            result = tmp2;
        }

        // Mmod = (Mmod * Mmod) % mod
        BigBinary sq = BigBinary_multiplyEgypt(&Mmod, &Mmod);
        BigBinary sq2 = BigBinary_mod(sq, mod);

        libereBigBinary(&Mmod);
        libereBigBinary(&sq);

        Mmod = sq2;

        // exp = exp / 2
        exp >>= 1;
    }

    return result;
}


BigBinary BigBinary_multiplyEgypt(const BigBinary *A, const BigBinary *B) {

    // Si l’un des deux est nul → résultat = 0
    if (A->Signe == 0 || B->Signe == 0)
        return initBigBinaryVide();

    // Copies locales
    BigBinary a = copieBigBinary(A);
    BigBinary b = copieBigBinary(B);

    // Résultat = 0
    BigBinary R = initBigBinaryVide();

    // Algorithme égyptien
    while (b.Signe != 0) {

        // Si b est impair → ajouter a à R
        if (!estPair(&b)) {
            BigBinary tmp = additionBigBinary(&R, &a);
            libereBigBinary(&R);
            R = tmp;
        }

        // a = a * 2
        BigBinary newA = multiplyBy2(&a);
        libereBigBinary(&a);
        a = newA;

        // b = b / 2
        divisePar2(&b);
    }

    // Nettoyage
    libereBigBinary(&a);
    libereBigBinary(&b);

    return R;
}

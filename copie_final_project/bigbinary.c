#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bigbinary.h"

// ========================================
// PHASE 1 : FONCTIONS DE BASE
// ========================================

BigBinary initBigBinaryVide(void) {
    BigBinary nb;
    nb.Taille = 1;
    nb.Signe = 0;
    nb.Tdigits = (int*)malloc(sizeof(int));
    if (!nb.Tdigits) exit(1);
    nb.Tdigits[0] = 0;
    return nb;
}

BigBinary initBigBinaryDepuisChaine(const char *chaine) {
    if (chaine == NULL || *chaine == '\0')
        return initBigBinaryVide();

    int longueur = strlen(chaine);
    BigBinary nb;
    nb.Tdigits = (int*)malloc(longueur * sizeof(int));
    if (!nb.Tdigits) exit(1);

    nb.Taille = longueur;
    nb.Signe = +1;
    int allZero = 1;

    for (int i = 0; i < longueur; i++) {
        nb.Tdigits[i] = (chaine[i] == '1') ? 1 : 0;
        if (nb.Tdigits[i] == 1)
            allZero = 0;
    }

    if (allZero)
        nb.Signe = 0;

    return nb;
}

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

void divisePar2(BigBinary *nb) {
    if (nb == NULL || nb->Signe == 0 || nb->Taille == 0)
        return;

    int newSize = nb->Taille - 1;

    if (newSize == 0) {
        nb->Tdigits[0] = 0;
        nb->Taille = 1;
        nb->Signe = 0;
        return;
    }

    int *newArr = (int*)malloc(sizeof(int) * newSize);
    for (int i = 0; i < newSize; i++) {
        newArr[i] = nb->Tdigits[i];
    }

    free(nb->Tdigits);
    nb->Tdigits = newArr;
    nb->Taille = newSize;

    int allZero = 1;
    for (int i = 0; i < nb->Taille; i++)
        if (nb->Tdigits[i] == 1)
            allZero = 0;

    nb->Signe = allZero ? 0 : +1;
}

void libereBigBinary(BigBinary *nb) {
    if (nb == NULL) return;
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B) {
    int nA = A ? A->Taille : 0;
    int nB = B ? B->Taille : 0;
    int n = (nA > nB ? nA : nB);
    int cap = n + 1;

    BigBinary R;
    R.Taille = cap;
    R.Signe = +1;
    R.Tdigits = (int*)malloc(sizeof(int) * cap);
    if (!R.Tdigits) exit(1);
    for (int i = 0; i < cap; ++i) R.Tdigits[i] = 0;

    int iA = nA - 1;
    int iB = nB - 1;
    int iR = cap - 1;
    int carry = 0;

    while (iA >= 0 || iB >= 0 || carry) {
        int bitA = (iA >= 0) ? A->Tdigits[iA] : 0;
        int bitB = (iB >= 0) ? B->Tdigits[iB] : 0;
        int s = bitA + bitB + carry;

        R.Tdigits[iR] = (s & 1);
        carry = (s >> 1);

        iA--; iB--; iR--;
    }

    // Normalisation
    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0) first++;

    if (first > 0) {
        int newLen = R.Taille - first;
        int *newArr = (int*)malloc(sizeof(int) * newLen);
        if (!newArr) exit(1);
        for (int k = 0; k < newLen; ++k) newArr[k] = R.Tdigits[first + k];
        free(R.Tdigits);
        R.Tdigits = newArr;
        R.Taille = newLen;
    }

    int allZero = 1;
    for (int i = 0; i < R.Taille; ++i)
        if (R.Tdigits[i] == 1) {
            allZero = 0;
            break;
        }
    R.Signe = allZero ? 0 : +1;

    return R;
}

BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B) {
    BigBinary R;
    R.Taille = A->Taille;
    R.Signe = +1;
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

    // Normalisation
    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0) first++;

    int newLen = R.Taille - first;
    int *newArr = malloc(newLen * sizeof(int));
    for (int k = 0; k < newLen; k++)
        newArr[k] = R.Tdigits[first + k];

    free(R.Tdigits);
    R.Tdigits = newArr;
    R.Taille = newLen;
    R.Signe = (newLen == 1 && newArr[0] == 0) ? 0 : +1;

    return R;
}

int Egal(const BigBinary *A, const BigBinary *B) {
    if (A->Signe != B->Signe) return 0;
    if (A->Taille != B->Taille) return 0;

    for (int i = 0; i < A->Taille; ++i) {
        if (A->Tdigits[i] != B->Tdigits[i]) return 0;
    }

    return 1;
}

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

// ========================================
// PHASE 2 : FONCTIONS AVANCÉES
// ========================================

BigBinary copieBigBinary(const BigBinary *A) {
    BigBinary C;
    C.Taille = A->Taille;
    C.Signe = A->Signe;
    C.Tdigits = (int*)malloc(sizeof(int) * C.Taille);

    for (int i = 0; i < C.Taille; i++)
        C.Tdigits[i] = A->Tdigits[i];

    return C;
}

int estPair(const BigBinary *A) {
    if (A->Signe == 0) return 1;
    return (A->Tdigits[A->Taille - 1] == 0);
}

BigBinary multiplyBy2(const BigBinary *A) {
    BigBinary R;
    R.Taille = A->Taille + 1;
    R.Signe = A->Signe;
    R.Tdigits = malloc(sizeof(int) * R.Taille);

    for (int i = 0; i < A->Taille; i++)
        R.Tdigits[i] = A->Tdigits[i];

    R.Tdigits[R.Taille - 1] = 0;

    return R;
}

BigBinary BigBinary_PGCD(BigBinary A, BigBinary B) {
    if (A.Signe == 0) return copieBigBinary(&B);
    if (B.Signe == 0) return copieBigBinary(&A);

    BigBinary a = copieBigBinary(&A);
    BigBinary b = copieBigBinary(&B);

    int k = 0;

    while (estPair(&a) && estPair(&b)) {
        divisePar2(&a);
        divisePar2(&b);
        k++;
    }

    while (estPair(&a)) {
        divisePar2(&a);
    }

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

    while (k > 0) {
        BigBinary tmp = multiplyBy2(&a);
        libereBigBinary(&a);
        a = tmp;
        k--;
    }

    return a;
}

BigBinary BigBinary_mod(BigBinary A, BigBinary B) {
    if (B.Signe == 0) {
        printf("Erreur : modulo par zero.\n");
        return initBigBinaryVide();
    }

    BigBinary R = copieBigBinary(&A);

    while (!Inferieur(&R, &B)) {
        BigBinary tmp = soustractionBigBinary(&R, &B);
        libereBigBinary(&R);
        R = tmp;
    }

    return R;
}

BigBinary BigBinary_multiply(const BigBinary *A, const BigBinary *B) {
    if (A->Signe == 0 || B->Signe == 0)
        return initBigBinaryVide();

    int nA = A->Taille;
    int nB = B->Taille;
    int nR = nA + nB;

    BigBinary R;
    R.Taille = nR;
    R.Signe = +1;
    R.Tdigits = calloc(nR, sizeof(int));

    for (int iB = nB - 1; iB >= 0; iB--) {
        if (B->Tdigits[iB] == 1) {
            int shift = (nB - 1 - iB);

            for (int iA = nA - 1; iA >= 0; iA--) {
                int posR = (nR - 1) - (nA - 1 - iA) - shift;
                R.Tdigits[posR] += A->Tdigits[iA];
            }
        }
    }

    for (int i = nR - 1; i > 0; i--) {
        if (R.Tdigits[i] > 1) {
            R.Tdigits[i - 1] += R.Tdigits[i] / 2;
            R.Tdigits[i] %= 2;
        }
    }

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

BigBinary BigBinary_multiplyEgypt(const BigBinary *A, const BigBinary *B) {
    if (A->Signe == 0 || B->Signe == 0)
        return initBigBinaryVide();

    BigBinary a = copieBigBinary(A);
    BigBinary b = copieBigBinary(B);
    BigBinary R = initBigBinaryVide();

    while (b.Signe != 0) {
        if (!estPair(&b)) {
            BigBinary tmp = additionBigBinary(&R, &a);
            libereBigBinary(&R);
            R = tmp;
        }

        BigBinary newA = multiplyBy2(&a);
        libereBigBinary(&a);
        a = newA;

        divisePar2(&b);
    }

    libereBigBinary(&a);
    libereBigBinary(&b);

    return R;
}

BigBinary BigBinary_expMod(BigBinary M, int exp, BigBinary mod) {
    BigBinary result = initBigBinaryDepuisChaine("1");
    BigBinary Mmod = BigBinary_mod(M, mod);

    while (exp > 0) {
        if (exp & 1) {
            BigBinary tmp = BigBinary_multiplyEgypt(&result, &Mmod);
            BigBinary tmp2 = BigBinary_mod(tmp, mod);
            libereBigBinary(&result);
            libereBigBinary(&tmp);
            result = tmp2;
        }

        BigBinary sq = BigBinary_multiplyEgypt(&Mmod, &Mmod);
        BigBinary sq2 = BigBinary_mod(sq, mod);
        libereBigBinary(&Mmod);
        libereBigBinary(&sq);
        Mmod = sq2;

        exp >>= 1;
    }

    libereBigBinary(&Mmod);
    return result;
}

// ========================================
// PHASE 3 : RSA AVEC BIGBINARY
// ========================================

BigBinary BigBinary_expModBigBinary(BigBinary M, BigBinary E, BigBinary N) {
    BigBinary result = initBigBinaryDepuisChaine("1");
    BigBinary base = BigBinary_mod(M, N);

    for (int i = 0; i < E.Taille; i++) {
        BigBinary sq1 = BigBinary_multiplyEgypt(&result, &result);
        BigBinary sq2 = BigBinary_mod(sq1, N);
        libereBigBinary(&result);
        libereBigBinary(&sq1);
        result = sq2;

        if (E.Tdigits[i] == 1) {
            BigBinary tmp1 = BigBinary_multiplyEgypt(&result, &base);
            BigBinary tmp2 = BigBinary_mod(tmp1, N);
            libereBigBinary(&result);
            libereBigBinary(&tmp1);
            result = tmp2;
        }
    }

    libereBigBinary(&base);
    return result;
}

BigBinary BigBinary_RSA_Encrypt(BigBinary M, BigBinary E, BigBinary N) {
    return BigBinary_expModBigBinary(M, E, N);
}

BigBinary BigBinary_RSA_Decrypt(BigBinary C, BigBinary D, BigBinary N) {
    return BigBinary_expModBigBinary(C, D, N);
}

BigBinary intToBigBinary(unsigned int n) {
    if (n == 0) {
        return initBigBinaryDepuisChaine("0");
    }

    unsigned int temp = n;
    int nbBits = 0;
    while (temp > 0) {
        nbBits++;
        temp >>= 1;
    }

    char *binStr = (char*)malloc((nbBits + 1) * sizeof(char));
    binStr[nbBits] = '\0';

    temp = n;
    for (int i = nbBits - 1; i >= 0; i--) {
        binStr[i] = (temp & 1) ? '1' : '0';
        temp >>= 1;
    }

    BigBinary result = initBigBinaryDepuisChaine(binStr);
    free(binStr);

    return result;
}

unsigned int bigBinaryToInt(BigBinary bb) {
    unsigned int result = 0;

    for (int i = 0; i < bb.Taille; i++) {
        result = (result << 1) | bb.Tdigits[i];
    }

    return result;
}
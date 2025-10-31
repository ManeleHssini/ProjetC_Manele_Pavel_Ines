
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ===============================
// Structure BigBinary
// ===============================
// Représentation d’un grand entier binaire
typedef struct {
    int *Tdigits;  // Tableau dynamique contenant les bits (0 ou 1)
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 si positif, -1 si négatif, 0 si nul
} BigBinary;

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

    // Vérification basique
    if (chaine == NULL || *chaine == '\0') {
        return initBigBinaryVide();
    }

    int longueur = strlen(chaine);
    nb.Taille = longueur;
    nb.Signe = +1; // par défaut positif
    nb.Tdigits = (int*)malloc(longueur * sizeof(int));
    if (!nb.Tdigits) exit(1);

    // On parcourt la chaîne et on stocke chaque caractère sous forme d'entier
    for (int i = 0; i < longueur; i++) {
        if (chaine[i] == '0')
            nb.Tdigits[i] = 0;
        else if (chaine[i] == '1')
            nb.Tdigits[i] = 1;
        else
            nb.Tdigits[i] = 0; // si autre chose que 0/1, on met 0 par défaut
    }

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

    // Si le nombre est nul ou vide, on ne fait rien
    if (nb == NULL || nb->Signe == 0 || nb->Taille == 0) {
        return;
    }

    // Cas particulier : si le nombre a un seul bit
    if (nb->Taille == 1) {
        nb->Tdigits[0] = 0;
        nb->Signe = 0;
        return;
    }

    // Décalage des bits vers la droite (division binaire)
    for (int i = nb->Taille - 1; i > 0; i--) {
        nb->Tdigits[i] = nb->Tdigits[i - 1];
    }

    // Le bit de gauche devient 0 après la division
    nb->Tdigits[0] = 0;

    // On réduit la taille du nombre de 1 (on supprime le dernier bit)
    nb->Taille--;

    // Si le résultat ne contient plus que des 0, on met le signe à 0
    int zero = 1;
    for (int i = 0; i < nb->Taille; i++) {
        if (nb->Tdigits[i] == 1) {
            zero = 0;
            break;
        }
    }
    if (zero) {
        nb->Signe = 0;
    }
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
    // Le résultat ne peut pas avoir plus de bits que A
    BigBinary R;
    R.Taille  = A->Taille;
    R.Signe   = +1; // ajusté après si résultat == 0
    R.Tdigits = (int*)malloc(sizeof(int) * R.Taille);
    if (!R.Tdigits) exit(1);
    for (int i = 0; i < R.Taille; ++i) R.Tdigits[i] = 0;

    int iA = A->Taille - 1;   // LSB de A
    int iB = B->Taille - 1;   // LSB de B
    int iR = R.Taille - 1;    // LSB de R
    int borrow = 0;           // emprunt (0 ou 1)

    // Soustraction bit à bit de droite à gauche
    while (iA >= 0) {
        int a = A->Tdigits[iA];
        int b = (iB >= 0) ? B->Tdigits[iB] : 0;

        int diff = a - borrow - b;
        if (diff >= 0) {
            R.Tdigits[iR] = diff;
            borrow = 0;
        } else {
            // on "emprunte" 1 (en binaire, emprunter = +2 sur le bit courant)
            R.Tdigits[iR] = diff + 2;
            borrow = 1;
        }

        iA--; iB--; iR--;
    }

    // --- Normalisation : enlever les zéros de tête inutiles ---
    int first = 0;
    while (first < R.Taille - 1 && R.Tdigits[first] == 0) first++;

    if (first > 0) {
        int newLen = R.Taille - first;
        int *newArr = (int*)malloc(sizeof(int) * newLen);
        if (!newArr) exit(1);
        for (int k = 0; k < newLen; ++k) newArr[k] = R.Tdigits[first + k];
        free(R.Tdigits);
        R.Tdigits = newArr;
        R.Taille  = newLen;
    }

    // Signe = 0 si le résultat est exactement 0, sinon +1
    int allZero = 1;
    for (int i = 0; i < R.Taille; ++i) if (R.Tdigits[i] == 1) { allZero = 0; break; }
    R.Signe = allZero ? 0 : +1;

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
    if (A->Signe == 0 && B->Signe != 0) return 1;  // 0 < positif
    if (A->Signe != 0 && B->Signe == 0) return 0;  // positif > 0

    if (A->Taille < B->Taille) return 1;
    if (A->Taille > B->Taille) return 0;

    for (int i = 0; i < A->Taille; ++i) {
        if (A->Tdigits[i] < B->Tdigits[i]) return 1;   // A[i] = 0, B[i] = 1
        if (A->Tdigits[i] > B->Tdigits[i]) return 0;   // A[i] = 1, B[i] = 0
    }

    return 0;
}


int main() {
    // Tests d'initialisation
    BigBinary a = initBigBinaryDepuisChaine("1010"); // 10
    BigBinary b = initBigBinaryDepuisChaine("0011"); // 3

    printf("a = "); afficheBigBinary(a);
    printf("b = "); afficheBigBinary(b);

    // Addition
    BigBinary s = additionBigBinary(&a, &b);
    printf("a + b = ");
    afficheBigBinary(s);

    // Soustraction
    BigBinary d = soustractionBigBinary(&a, &b);
    printf("a - b = ");
    afficheBigBinary(d);

    // Division par 2
    printf("a / 2 = ");
    divisePar2(&a);
    afficheBigBinary(a);

    // Comparaisons
    printf("a == b ? %d\n", Egal(&a, &b));
    printf("a < b ? %d\n", Inferieur(&a, &b));

    // Libération mémoire
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&s);
    libereBigBinary(&d);

    return 0;
}

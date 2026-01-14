/*
 * ============================================================================
 * PROJET RSA - PHASE 3 COMPLÈTE
 * Tout dans un seul fichier pour simplifier
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// CONSTANTES ET STRUCTURE
// ============================================================================

#define MAX_DIGITS 100
#define BASE 10

typedef struct {
    unsigned int digits[MAX_DIGITS];
    int taille;
} GrandEntier;

// ============================================================================
// FONCTIONS UTILITAIRES DE BASE
// ============================================================================

// Initialise un GrandEntier à zéro
void initGrandEntier(GrandEntier *a) {
    for (int i = 0; i < MAX_DIGITS; i++) {
        a->digits[i] = 0;
    }
    a->taille = 1;
}

// Crée un GrandEntier à partir d'un unsigned int
void creerGrandEntier(GrandEntier *resultat, unsigned int valeur) {
    initGrandEntier(resultat);
    
    if (valeur == 0) {
        resultat->taille = 1;
        resultat->digits[0] = 0;
        return;
    }
    
    int i = 0;
    while (valeur > 0) {
        resultat->digits[i] = valeur % BASE;
        valeur = valeur / BASE;
        i++;
    }
    resultat->taille = i;
}

// Copie un GrandEntier dans un autre
void copierGrandEntier(GrandEntier *destination, GrandEntier *source) {
    destination->taille = source->taille;
    for (int i = 0; i < source->taille; i++) {
        destination->digits[i] = source->digits[i];
    }
    // Mettre à zéro les chiffres restants
    for (int i = source->taille; i < MAX_DIGITS; i++) {
        destination->digits[i] = 0;
    }
}

// Affiche un GrandEntier
void afficherGrandEntier(GrandEntier a) {
    for (int i = a.taille - 1; i >= 0; i--) {
        printf("%u", a.digits[i]);
    }
}

// Compare deux GrandEntiers: retourne 1 si a > b, -1 si a < b, 0 si a == b
int comparer(GrandEntier *a, GrandEntier *b) {
    if (a->taille > b->taille) return 1;
    if (a->taille < b->taille) return -1;
    
    for (int i = a->taille - 1; i >= 0; i--) {
        if (a->digits[i] > b->digits[i]) return 1;
        if (a->digits[i] < b->digits[i]) return -1;
    }
    return 0;
}

// ============================================================================
// ADDITION (Phase 1 - vous devriez l'avoir)
// ============================================================================

void addition(GrandEntier *resultat, GrandEntier *a, GrandEntier *b) {
    initGrandEntier(resultat);
    unsigned int retenue = 0;
    int maxTaille = (a->taille > b->taille) ? a->taille : b->taille;
    
    for (int i = 0; i < maxTaille || retenue > 0; i++) {
        unsigned int somme = retenue;
        if (i < a->taille) somme += a->digits[i];
        if (i < b->taille) somme += b->digits[i];
        
        resultat->digits[i] = somme % BASE;
        retenue = somme / BASE;
        resultat->taille = i + 1;
    }
}

// ============================================================================
// SOUSTRACTION (Phase 1 - vous devriez l'avoir)
// ============================================================================

void soustraction(GrandEntier *resultat, GrandEntier *a, GrandEntier *b) {
    initGrandEntier(resultat);
    int emprunt = 0;
    
    for (int i = 0; i < a->taille; i++) {
        int diff = a->digits[i] - emprunt;
        if (i < b->taille) diff -= b->digits[i];
        
        if (diff < 0) {
            diff += BASE;
            emprunt = 1;
        } else {
            emprunt = 0;
        }
        
        resultat->digits[i] = diff;
    }
    
    // Ajuster la taille
    resultat->taille = a->taille;
    while (resultat->taille > 1 && resultat->digits[resultat->taille - 1] == 0) {
        resultat->taille--;
    }
}

// ============================================================================
// MULTIPLICATION (Phase 1 - vous devriez l'avoir)
// ============================================================================

void multiplication(GrandEntier *resultat, GrandEntier *a, GrandEntier *b) {
    initGrandEntier(resultat);
    
    for (int i = 0; i < a->taille; i++) {
        unsigned int retenue = 0;
        for (int j = 0; j < b->taille || retenue > 0; j++) {
            unsigned int produit = resultat->digits[i + j] + retenue;
            if (j < b->taille) {
                produit += a->digits[i] * b->digits[j];
            }
            resultat->digits[i + j] = produit % BASE;
            retenue = produit / BASE;
        }
    }
    
    // Calculer la taille
    resultat->taille = a->taille + b->taille;
    while (resultat->taille > 1 && resultat->digits[resultat->taille - 1] == 0) {
        resultat->taille--;
    }
}

// ============================================================================
// MODULO (Phase 2 - vous devriez l'avoir)
// ============================================================================

void modulo(GrandEntier *resultat, GrandEntier *a, GrandEntier *N) {
    copierGrandEntier(resultat, a);
    
    // Soustraire N tant que resultat >= N
    while (comparer(resultat, N) >= 0) {
        GrandEntier temp;
        soustraction(&temp, resultat, N);
        copierGrandEntier(resultat, &temp);
    }
}

// ============================================================================
// PHASE 3 : EXPONENTIATION RAPIDE AVEC GRANDENTIER
// ============================================================================

// Fonction auxiliaire: élève M à la puissance exposant (où exposant est un unsigned int)
// VERSION OPTIMISÉE avec modulo intégré
void puissanceModulo(GrandEntier *resultat, GrandEntier *M, unsigned int exposant, GrandEntier *N) {
    creerGrandEntier(resultat, 1);
    
    if (exposant == 0) {
        return;
    }
    
    GrandEntier base;
    copierGrandEntier(&base, M);
    
    // IMPORTANT: Prendre modulo dès le début
    GrandEntier temp;
    modulo(&temp, &base, N);
    copierGrandEntier(&base, &temp);
    
    while (exposant > 0) {
        if (exposant % 2 == 1) {
            // Si exposant est impair, multiplier par base
            GrandEntier temp1;
            multiplication(&temp1, resultat, &base);
            // MODULO après chaque multiplication!
            modulo(resultat, &temp1, N);
        }
        
        // base = base^2
        GrandEntier temp2;
        multiplication(&temp2, &base, &base);
        // MODULO après chaque multiplication!
        modulo(&base, &temp2, N);
        
        exposant = exposant / 2;
    }
}

// ============================================================================
// ALGORITHME PRINCIPAL: Exponentiation Modulaire avec E de type GrandEntier
// ============================================================================

/*
 * Calcule M^E mod N où E est un GrandEntier
 * 
 * Algorithme "Maison":
 * Si E = e3*10^3 + e2*10^2 + e1*10^1 + e0 (exemple: E=5234)
 * Alors M^E = M^e0 * (M^10)^e1 * (M^100)^e2 * (M^1000)^e3
 */
void exponentiationModulaireGrandEntier(GrandEntier *resultat, GrandEntier *M, GrandEntier *E, GrandEntier *N) {
    printf("  [Debug] Début calcul M^E mod N...\n");
    printf("  [Debug] E a %d chiffres\n", E->taille);
    
    // Résultat = 1 au départ
    creerGrandEntier(resultat, 1);
    
    // Puissance courante = M mod N
    printf("  [Debug] Calcul M mod N...\n");
    GrandEntier puissanceCourante;
    modulo(&puissanceCourante, M, N);
    printf("  [Debug] M mod N calculé!\n");
    
    // Parcourir chaque chiffre de E (de droite à gauche: digits[0], digits[1], etc.)
    for (int i = 0; i < E->taille; i++) {
        unsigned int chiffre = E->digits[i];
        printf("  [Debug] Chiffre %d/%d : valeur = %u\n", i+1, E->taille, chiffre);
        
        // Si le chiffre n'est pas 0, calculer (puissanceCourante)^chiffre mod N
        if (chiffre > 0) {
            // Élever puissanceCourante à la puissance chiffre AVEC MODULO
            printf("    -> Calcul puissance^%u mod N (optimisé)...\n", chiffre);
            GrandEntier temp1;
            puissanceModulo(&temp1, &puissanceCourante, chiffre, N);
            printf("    -> Puissance mod N calculée!\n");
            
            // Multiplier au résultat
            printf("    -> Multiplication...\n");
            GrandEntier temp2;
            multiplication(&temp2, resultat, &temp1);
            printf("    -> Multiplication faite!\n");
            
            // Prendre modulo N et sauvegarder dans resultat
            printf("    -> Modulo final...\n");
            modulo(resultat, &temp2, N);
            printf("    -> Modulo final fait!\n");
        }
        
        // Préparer pour le prochain chiffre: puissanceCourante = (puissanceCourante)^BASE mod N
        if (i < E->taille - 1) {
            printf("    -> Préparation chiffre suivant (puissance^%d mod N)...\n", BASE);
            puissanceModulo(&puissanceCourante, &puissanceCourante, BASE, N);
            printf("    -> Prêt pour chiffre suivant!\n");
        }
    }
    printf("  [Debug] Exponentiation terminée!\n");
}

// ============================================================================
// FONCTIONS RSA
// ============================================================================

// Chiffrement RSA: C = M^E mod N
void chiffrementRSA(GrandEntier *C, GrandEntier *M, GrandEntier *E, GrandEntier *N) {
    printf("Chiffrement en cours...\n");
    exponentiationModulaireGrandEntier(C, M, E, N);
}

// Déchiffrement RSA: M = C^D mod N
void dechiffrementRSA(GrandEntier *M, GrandEntier *C, GrandEntier *D, GrandEntier *N) {
    printf("Déchiffrement en cours...\n");
    exponentiationModulaireGrandEntier(M, C, D, N);
}

// ============================================================================
// FONCTION MAIN - TESTS
// ============================================================================

int main() {
    printf("======================================\n");
    printf("    TEST RSA PHASE 3 COMPLETE\n");
    printf("======================================\n\n");
    
    // Exemple du document:
    // p = 1009, q = 1201
    // N = 1211809
    // E = 101
    // D = 251501
    // Message M = 99999
    // Résultat attendu: C = 561752
    
    GrandEntier M, E, N, D, C, M_dechiffre;
    
    // Créer les nombres
    creerGrandEntier(&M, 99999);
    creerGrandEntier(&E, 101);
    creerGrandEntier(&N, 1211809);
    creerGrandEntier(&D, 251501);
    
    // Afficher le message original
    printf("Message original M = ");
    afficherGrandEntier(M);
    printf("\n\n");
    
    // Afficher la clé publique
    printf("Clé publique (N, E):\n");
    printf("  N = ");
    afficherGrandEntier(N);
    printf("\n");
    printf("  E = ");
    afficherGrandEntier(E);
    printf("\n\n");
    
    // CHIFFREMENT
    printf("--- CHIFFREMENT ---\n");
    chiffrementRSA(&C, &M, &E, &N);
    printf("Message chiffré C = ");
    afficherGrandEntier(C);
    printf("\n");
    printf("(Attendu: 561752)\n\n");
    
    // DÉCHIFFREMENT
    printf("--- DÉCHIFFREMENT ---\n");
    dechiffrementRSA(&M_dechiffre, &C, &D, &N);
    printf("Message déchiffré = ");
    afficherGrandEntier(M_dechiffre);
    printf("\n");
    printf("(Devrait être: 99999)\n\n");
    
    // Vérifier si le déchiffrement a fonctionné
    if (comparer(&M, &M_dechiffre) == 0) {
        printf("✓ SUCCESS: Le déchiffrement a fonctionné!\n");
    } else {
        printf("✗ ERREUR: Le déchiffrement n'a pas fonctionné correctement.\n");
    }
    
    printf("\n======================================\n");
    
    return 0;
}

/*
 * ============================================================================
 * INSTRUCTIONS DE COMPILATION ET UTILISATION
 * ============================================================================
 * 
 * Pour compiler:
 *   gcc phase3.c -o phase3
 * 
 * Pour exécuter:
 *   ./phase3
 * 
 * ============================================================================
 * EXPLICATIONS
 * ============================================================================
 * 
 * 1. STRUCTURE: Tout est dans un seul fichier, pas besoin de .h
 * 
 * 2. ORDRE: 
 *    - Includes et defines en haut
 *    - Structure typedef
 *    - Toutes les fonctions
 *    - main() à la fin
 * 
 * 3. L'ALGORITHME PRINCIPAL (exponentiationModulaireGrandEntier):
 *    - Parcourt E chiffre par chiffre
 *    - Pour chaque chiffre e_i, calcule (M^(BASE^i))^e_i
 *    - Multiplie tous les résultats ensemble modulo N
 * 
 * 4. FONCTIONS QUE VOUS AVIEZ DÉJÀ (Phase 1 et 2):
 *    - addition, soustraction, multiplication
 *    - modulo
 *    Je les ai incluses pour que le code soit complet et compile
 * 
 * 5. SI VOUS AVEZ DÉJÀ CES FONCTIONS:
 *    Remplacez-les par vos propres implémentations!
 *    L'important c'est exponentiationModulaireGrandEntier()
 * 
 * ============================================================================
 */
# Projet BigBinary — README

## Vue d'ensemble
Ce projet implémente des opérations sur de grands entiers représentés en binaire (`BigBinary`) et inclut une démonstration du chiffrement RSA à titre pédagogique.

Fichiers principaux :
- `bigbinary.h`  : définitions de la structure `BigBinary` et prototypes des fonctions.
- `bigbinary.c`  : implémentation des phases du projet : opérations de base (Phase 1), fonctions avancées (Phase 2) et fonctions RSA (Phase 3).
- `main.c`       : interface console (menu), tests manuels et automatisés.

Auteurs : Manele, Pavel, Ines

---

## Compilation
Placez-vous dans le dossier `copie_final_project` puis compilez avec GCC :

```bash
gcc -g bigbinary.c main.c -o main
# Sous Windows :
gcc -g bigbinary.c main.c -o main.exe
```

Vous pouvez aussi utiliser la tâche Visual Studio Code fournie (`C/C++: gcc.exe build active file`).

---

## Exécution
L'exécutable est interactif :

```bash
./main      # Linux / WSL
main.exe    # Windows
```

Au démarrage, un menu propose plusieurs opérations (entrer des nombres binaires sous forme de chaînes de '0' et '1' ou, pour RSA manuel, des valeurs en décimal) :
- 1 : Addition (A + B)
- 2 : Soustraction (A - B) — uniquement si A >= B
- 3 : Multiplication
- 4 : Multiplication égyptienne (méthode alternative)
- 5 : Division par 2
- 6 : PGCD (algorithme de Stein adapté en base 2)
- 7 : A mod B
- 8 : Exponentiation modulaire (M^exp mod N) — exposant en décimal
- 9 : Test RSA manuel (entrer M, E, N, D en décimal)
- 10: Tests automatiques (suite de cas-tests prédéfinis)
- 0 : Quitter

---

## Détails du fichier `main.c`
Le fichier `main.c` fournit :
- Un menu interactif et la gestion des entrées utilisateurs via `scanf`.
- La fonction `testsAutomatiques()` qui exécute une série de cas pour valider :
  - Opérations de base (addition, soustraction, division par 2, comparaisons)
  - Opcodes de la Phase 2 (PGCD, multiplications, modulo)
  - Phase 3 : un test RSA complet avec paramètres fixes (M=99999, E=101, N=1211809, D=251501)
- Des messages explicites et des valeurs « attendues » pour faciliter la vérification visuelle.

---

## Tests automatiques vs tests manuels
### Tests automatiques (option 10)
- Exécutent des cas prédéfinis et affichent les résultats attendus à l'écran.
- Utile pour vérifier rapidement l'intégrité des fonctions essentielles (Phase 1 et Phase 2) et voir un exemple de RSA chiffré/déchiffré.

### Tests manuels (menu)
- L'utilisateur saisit directement les entrées via `scanf`.
- Permet de tester des cas personnalisés : opérations binaires (entrées en binaire) et RSA (entrées en décimal).

Remarque importante : le test RSA automatique présent dans `testsAutomatiques()` montre un chiffrement/déchiffrement cohérent pour les paramètres fournis. Cependant, le test RSA manuel (option 9) peut échouer si les paramètres E, D, N fournis manuellement ne sont pas cohérents (par exemple si D n'est pas l'inverse modulaire de E modulo phi(N), ou si N/E/D ne correspondent pas à une paire de clés valides). En pratique, si vous observez une erreur lors du test RSA manuel, vérifiez les paramètres suivants :
- N doit être le produit de deux nombres premiers (p × q)
- E et D doivent être choisis tels que E·D ≡ 1 (mod φ(N))
- M doit être plus petit que N pour l'exemple actuel

Si nécessaire, on peut ajouter une routine pour vérifier la validité des paramètres RSA et afficher un message d'erreur plus explicite.

---

## Phase 1 — Fonctions de base
- `initBigBinaryVide()` : crée la valeur 0 (`BigBinary` avec `Taille=1`, `Tdigits[0]=0`, `Signe=0`).
- `initBigBinaryDepuisChaine(const char *)` : construit un `BigBinary` à partir d'une chaîne de '0' et '1' (alloue `Tdigits`).
- `afficheBigBinary(BigBinary)` : affiche le nombre en binaire (affiche `-` si `Signe == -1`).
- `divisePar2(BigBinary *)` : division entière par 2 (décalage à droite, ajuste `Taille` et `Signe`).
- `additionBigBinary(const BigBinary*, const BigBinary*)` : addition binaire (renvoie un `BigBinary` alloué, format normalisé).
- `soustractionBigBinary(const BigBinary*, const BigBinary*)` : soustraction (A - B), renvoie un `BigBinary` alloué et normalisé ; la vérification A >= B est effectuée côté appelant dans le menu.
- `Egal(const BigBinary*, const BigBinary*)` : test d'égalité (1 si égaux, 0 sinon).
- `Inferieur(const BigBinary*, const BigBinary*)` : test (1 si A < B, 0 sinon).
- `libereBigBinary(BigBinary *)` : libère la mémoire interne d'un `BigBinary`.

---

## Phase 2 — Fonctions avancées
- `estPair(const BigBinary *)` : vérifie si le nombre est pair.
- `copieBigBinary(const BigBinary *)` : copie profonde d'un `BigBinary` (renvoie un `BigBinary` alloué).
- `multiplyBy2(const BigBinary *)` : multiplication par 2 (décalage à gauche, ajoute un bit 0 de poids faible).
- `BigBinary_multiply(const BigBinary*, const BigBinary*)` : multiplication binaire avec propagation de retenues.
- `BigBinary_multiplyEgypt(const BigBinary*, const BigBinary*)` : multiplication par la méthode égyptienne (addition conditionnelle + décalage).
- `BigBinary_mod(BigBinary A, BigBinary B)` : calcule A mod B par soustractions itératives (renvoie `BigBinary` alloué).
- `BigBinary_PGCD(BigBinary A, BigBinary B)` : PGCD par algorithme de Stein (variante binaire), renvoie `BigBinary` alloué.
- `BigBinary_expMod(BigBinary M, int exp, BigBinary mod)` : exponentiation modulaire (exposant `int`).

---

## Fonctions — détails et remarques (signatures, comportement, contraintes)
Chaque fonction qui retourne un `BigBinary` alloue la mémoire interne (`Tdigits`) : il est impératif de libérer ces objets via `libereBigBinary()` lorsqu'ils ne sont plus nécessaires pour éviter des fuites mémoire.

Résumé des comportements et limitations :

- `BigBinary initBigBinaryDepuisChaine(const char *s)`
  - Description : crée un `BigBinary` à partir de la chaîne `s` (caractères `0`/`1`).
  - Remarque : l'entrée est supposée valide (aucune vérification approfondie des caractères non binaires).

- `BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B)`
  - Description : addition bit-à-bit avec gestion de la retenue ; normalisation du résultat (suppression des zéros de tête).

- `BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B)`
  - Description : calcule A - B ; la validité (A >= B) doit être assurée par l'appelant (le menu vérifie avec `Inferieur()`).

- `BigBinary BigBinary_mod(BigBinary A, BigBinary B)`
  - Description : calcule le reste de la division de A par B (méthode par soustractions répétées).
  - Limitation : approche peu efficace pour grands nombres (complexité élevée pour grands quotients).

- `BigBinary BigBinary_expMod(BigBinary M, int exp, BigBinary mod)` et `BigBinary BigBinary_expModBigBinary(BigBinary M, BigBinary E, BigBinary N)`
  - Description : exponentiation modulaire par exponentiation rapide (square-and-multiply) ; la version `BigBinary` parcourt les bits de l'exposant.

- Conversions : `BigBinary intToBigBinary(unsigned int)` et `unsigned int bigBinaryToInt(BigBinary)`
  - Limitation : conversions limitées à la taille d'un `unsigned int` (risque d'overflow si le `BigBinary` dépasse la capacité du type).

---

## Phase 3 — RSA (bonus)
- `BigBinary_expModBigBinary(M, E, N)` : exponentiation modulaire avec un exposant `BigBinary` (utilisée pour RSA avec exposants binaires)
- `BigBinary_RSA_Encrypt(M, E, N)` et `BigBinary_RSA_Decrypt(C, D, N)` utilisent cette fonction.

Notes pratiques :
- Les routines RSA sont implémentées pour démonstration et conviennent pour de petits entiers (stockés dans des `unsigned int` via `intToBigBinary` et `bigBinaryToInt`). Elles ne sont pas optimisées pour de grands nombres cryptographiques.
- Pour un usage réel, il faudrait :
  - Utiliser des représentations plus efficaces (grands entiers en base 2^32 ou 2^64),
  - Implémenter un algorithme d'exponentiation modulaire plus performant et sécurisé,
  - Ajouter des vérifications de validité des clés.

---

## Bonus — remarques de conformité et extensions
- Conformément au cahier des charges, les tests manuels via saisies `scanf` sont obligatoires : le menu interactif implémente ces saisies pour tous les cas demandés (Phase 1 à Phase 3).
- En extension (non exigée par le cahier), une suite de tests automatiques (option 10) a été ajoutée pour faciliter la vérification reproductible des fonctions et fournir des cas d'exemple (affiche aussi les résultats attendus).
- La partie RSA (Phase 3) est implémentée pour démonstration et incluse dans les tests automatiques ; lors d'un test manuel, un échec peut provenir de paramètres (E, D, N) incohérents fournis par l'utilisateur.

---

## Exemples d'utilisation
1) Addition : entrez `1011` puis `0110`, le programme affiche `10001`.
2) RSA manuel : entrez des paramètres cohérents (M < N, E et D inverses modulo φ(N)). Si le déchiffrement ne recouvre pas M, vérifiez la validité des clés.

---

## Remarques sur la mémoire
Toutes les fonctions qui allouent dynamiquement doivent être appariées avec `libereBigBinary()` pour éviter les fuites mémoire.


# Amélioration de l’immersion en réalité virtuelle


## Objectif

L'objectif principal de ce projet est de concevoir et de réaliser un système de Full-Body-Tracking (FBT) intégral en utilisant des technologies accessibles et peu coûteuses, afin d'améliorer l'immersion en réalité virtuelle (VR). Le projet inclut également la création de gants haptiques et un système d'eye-tracking pour une expérience VR complète.

## Description du projet

### Full-Body-Tracking

#### Concept

Recréer un système de full-body-tracking utilisant des caméras extérieures connectées à un ordinateur.

#### Méthodologie

Des trackers sous forme de QR-codes sont attachés à différentes parties du corps. Ces QR-codes, détectés par les caméras, permettent d'identifier la position et l'orientation des différentes parties du corps sans nécessiter d’alimentation des trackers.

#### Technologies utilisées

- Utilisation de vieux téléphones sur lesquels j'ai installé un système d’exploitation LineageOS pour optimiser la gestion de l'énergie, et dont je récupère le flux vidéo de leurs caméras.
- Le logiciel de détection de QR-codes AprilTag, développé par l'Université du Michigan, est utilisé pour le traitement des images. 
- L’impression 3D permettant la création de support pour les téléphones, et les attaches pour le corps.

### Gants haptiques

#### Concept

Créer des gants haptiques pour améliorer l'immersion tactile en VR.

#### Méthodoogie

Les gants utilisent des servo-moteurs pilotés par un ESP32. Les servo-moteurs contrôlent des fils attachés à des bagues pour les doigts. Lorsque les doigts se plient, les fils se tendent, simulant une résistance et offrant une sensation tactile réaliste. Le code pour l'ESP32 provient du projet open-source « LucidGloves », et les drivers utilisés sur l'ordinateur sont « OpenGloves », développés par Dan Willm.

#### Technologies utilisées

- ESP32, servo-moteurs, fils résistants, systèmes d'enrouleur pour la gestion des fils.
- Impression 3D pour les bagues ainsi que les supports des composants des gants.
- Code open-source LucidGloves, drivers OpenGloves.

### Eye-tracking

#### Concept

Ajouter un système d'eye-tracking pour améliorer l'interaction et l'immersion en VR.

#### Méthodologie

Utilisation de capteurs infrarouges pour détecter la position de l'iris par rapport à l'œil. Les données des capteurs sont traitées par un ESP32, avec le code tiré du projet open-source « OpenIris ».

#### Technologies utilisées

- Capteurs infrarouges, ESP32.
- Impression 3D pour les supports des capteurs dans le casque VR et de l’électronique.
- Logiciel OpenIris.

## Défis et solutions

### Detection et identification des QR-codes

#### Enjeu

Assurer une détection précise et rapide des QR-codes sur les différentes parties du corps.

#### Solution

Utiliser le logiciel AprilTag développé par l'Université du Michigan, qui offre des algorithmes de traitement d'image avancés.
Calibrer les caméras pour des performances optimales.

### Conception des gants haptiques

#### Enjeu

Créer un système mécanique fiable et confortable.

#### Solution

Prototyper plusieurs versions de gants et tester différents matériaux, plusieurs formes de supports afin d’améliorer l’ergonomie.

### Intégration de l'eye-tracking

#### Enjeu

Intégrer les capteurs infrarouges sans compromettre le confort et la fonctionnalité du casque VR.

#### Solution

Étudier les implémentations existantes dans OpenIris et adapter le design pour une intégration harmonieuse.

## Conclusion

Ce projet vise à repousser les limites de l'immersion en réalité virtuelle en combinant des technologies de tracking, de retour haptique et de suivi oculaire. En utilisant des composants accessibles et peu coûteux, je souhaite rendre ces avancées technologiques disponibles à un plus large public. Le casque Meta Quest 2 servira de base pour ces modifications, offrant une plateforme flexible et populaire pour l'expérimentation et le développement.
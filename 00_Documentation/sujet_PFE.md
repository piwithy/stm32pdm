# Traitement de signal audio embarqué temps réel sur carte STM32

### Dates du stage :
22 Septembre 2021 &rarr ; 30 Novembre 2021 (10 semaines)

### Livrables :
 - Démonstrateur : 26 Novembre 12 h
 - Rapport et Source : 29 Novembre 8 h

### Ingénieurs Pilotes :
 - [Irvin Probst](mailto:irvin.probst@ensta-bretagne.fr)
 - [Olivier Reynet](mailto:olivier.reynet@ensta-bretagne.fr)

### Objectif principal :
Lever les incertitudes sur l’acquisition audio temps réel sur carte stm32 avec microphone PDM.

<ul>
    <li>
        La cible STM32 est un standard de l’industrie embarquée. Les microphones numériques PDM représentent une innovation intéressante par rapport au niveau signal sur bruit. Ce projet de fin d’études a pour but de fournir une configuration clé en main de la carte STM32 afin : <br/>
        <ol>
            <li>
                D’acquérir un signal numérique audio en utilisant le DMA du processeur et en effectuant le filtrage indispensable aux microphones PDM.
            </li>
            <li>
                De générer un son préalablement acquis via le microphone PDM grâce au DAC
            </li>
        </ol>
    </li>
    <li>
        On s’intéressera particulièrement au réglage du microphone numérique afin de maîtriser (sans faire appel à des librairies statiques fermées) :
        <ol>
            <li>
                La fréquence d’échantillonnage,
            </li>
            <li>
                Le filtrage indispensable aux micros numériques (PDM vers PCM)
            </li>
            <li>
                La conversion au format PCM puis WAV sur la carte cible,
            </li>
            <li>
            Le fonctionnement global de la chaîne d’acquisition PDM (choix des fréquences d’horloge SAI en lien avec la fréquence d’échantillonnage du microphone, paramètres de la conversion PDM → PCM, etc.).
            </li>
        </ol>
    </li>
</ul>

### Démonstrateur :
Le démonstrateur final sera, au minimum, constitué d’une carte STM32, d’un microphone PDM et d’un haut-parleur ou casque. L’appui sur un bouton déclenchera l’enregistrement du son, un second appui déclenchera le rejeu du son.

### Matériel et code fournis :
 - Une ou plusieurs cartes STM32 et tout le nécessaire pour communiquer avec.
 - Un exemple d’acquisition via le DMA sur un microphone PDM avec filtrage « boite noire » via une bibliothèque STM32 peu documentée

### Reporting :
Hebdomadaire par email

### Modalité de livraison :
 - Le code et la documentation associée seront stockés sur la forge ([Gitlab](https://gitlab.ensta-bretagne.fr/projets-se/stm32pdm)).
 - Le rapport final sera remis en main propre aux Ingénieurs Pilotes.

<h1> STM32PDM </h1

Traitement de signal audio embarqué temps-réel sur carte STM32

# Contenu du dépot

| Dossier | Objectif |
|:--------|:---------|
|[00_Documentation](00_Documentation/readme.md) | Dossier contenant la documentation relative à la réalisation du projet (Carte Cible, µ-Controlleur, Exemples...) |
|[01_PCM_PDM_Toolbox](01_PCM_PDM_Toolbox/readme.md) | Dossier contenant divers scripts Python pour les conversions PCM PDM WAV|
|[02_Filtering](02_Filtering/readme.md)| Dossier contenant la librairie utilisé pour filtré le signal PDM, Le script Python pour générer la Look Up Table du filtre et un programme implémentant la librairie de filtrage|
|[03_Embedding](03_Embedding/readme.md)| Dossier contenant les programmes de demonstration pour la carte de cible |


# Objectif du Projet

Ce projet a pour objectif principal de lever les incertitudes sur l’acquisition audio temps réel sur carte stm32 avec microphone PDM. (cf. [sujet PFE](00_Documentation/sujet_PFE.md))

# Plateforme Cible

La cible du projet est une carte de "decouverte" ST Microelectronics utilisant Un µ-controlleur de la famille des STM32, cette carte et la [STM32F429I-DISC1](https://www.st.com/en/evaluation-tools/32f429idiscovery.html).

![stm232f429i-disc1](https://www.st.com/bin/ecommerce/api/image.PF259090.en.feature-description-include-personalized-no-cpn-medium.jpg)

Elle se base sur un µ-controlleur [STM32F429ZIT6U](https://www.st.com/en/microcontrollers-microprocessors/stm32f429zi.html)



# Auteur

Ce Projet a été créé par en 2021 par Pierre-Yves Jezegou. Dans le cadre d'un Projet de Fin d'étude de l'[ENSTA-Bretagne](https://ensta-bretagne.fr/)

# Licence

[BSD 3-Clause License](LICENSE)

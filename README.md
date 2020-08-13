# elf_packer

Etapes de réalisation du projet: 

- Extraire la table de segment, la table de section et l'en-tête elf
- Réécrire un fichier elf en se basant sur les données extraites (penser au padding)
- Ajouter une nouvelle section
- Chiffrer la section .text (voir plus, à réfléchir)
- Réaliser le code assembleur permettant de déchiffrer
- L'intégrer dans le fichier
- Changer le point d'entrée
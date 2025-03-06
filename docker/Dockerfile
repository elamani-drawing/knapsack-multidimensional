# Utiliser une image de base Ubuntu
FROM ubuntu:22.04

# Mettre à jour les paquets et installer Doxygen, Graphviz, GCC, et Make
RUN apt-get update && \
    apt-get install -y \
        doxygen \
        graphviz \
        git \
        make \
        gcc \
        build-essential \
        && rm -rf /var/lib/apt/lists/*

# Définir le répertoire de travail
WORKDIR /workspace

# Copier le code source dans le conteneur
COPY . /workspace

# Commande par défaut (lancer un shell)
CMD ["/bin/bash"]
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm


################################################# Itérations (k_perturbation) #######################################################

# Chargement
df = pd.read_csv('benchmark/vns_gloutonne_vs_aleatoire_k_perturbation.csv')

# Moyenne des valeurs et du temps selon k_perturbation
grouped = df.groupby(['filename', 'type', 'k_perturbation']).agg({
    'value': 'mean',
    'time': 'mean'
}).reset_index()

# Création des sous-graphes
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10), sharex=True)

# Boucle pour chaque combinaison fichier + type
for (filename, type_), group in grouped.groupby(['filename', 'type']):
    label = f'{filename} ({type_})'
    ax1.plot(group['k_perturbation'], group['value'], label=label)
    ax2.plot(group['k_perturbation'], group['time'], linestyle='--', label=label)

# Personnalisation
ax1.set_title("Évolution de la valeur selon le paramètre k (perturbation VNS)")
ax1.set_ylabel("Valeur")
ax1.grid(True)
ax1.legend()

ax2.set_title("Évolution du temps selon le paramètre k (perturbation VNS)")
ax2.set_xlabel("k (Perturbation)")
ax2.set_ylabel("Temps (s)")
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.show()

################################################## Comparaison Gloutonne vs Aléatoire (par k_perturbation) #######################################################

# Calcul des moyennes groupées par fichier et type
grouped = df.groupby(['filename', 'type']).agg({'value': 'mean', 'time': 'mean'}).reset_index()

# Séparer les données par type
gloutonne = grouped[grouped['type'] == 'vns_gloutonne']
aleatoire = grouped[grouped['type'] == 'vns_aleatoire']

# Indices pour les barres
x = range(len(gloutonne))

# Création du graphique
plt.figure(figsize=(14, 7))

# Barres des valeurs moyennes
plt.bar(x, gloutonne['value'], width=0.4, label='VNS Gloutonne', color='blue', align='center')
plt.bar(x, aleatoire['value'], width=0.4, label='VNS Aléatoire', color='red', align='edge')

# Labels et titre
plt.xlabel('Fichier')
plt.ylabel('Valeur moyenne')
plt.title('Comparaison des algorithmes VNS Gloutonne vs VNS Aléatoire (par fichier, moyennes sur k_perturbation)')

plt.xticks(x, gloutonne['filename'], rotation=90)
plt.legend()
plt.tight_layout()
plt.show()


################################################# Itérations #######################################################

# Chargement
df = pd.read_csv('benchmark/vns_gloutonne_vs_aleatoire_iteration.csv')

# Moyenne des valeurs et du temps
grouped = df.groupby(['filename', 'type', 'vns_iterations']).agg({
    'value': 'mean',
    'time': 'mean'
}).reset_index()

# Création des sous-graphes
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10), sharex=True)

# Boucle pour chaque combinaison fichier + type
for (filename, type_), group in grouped.groupby(['filename', 'type']):
    label = f'{filename} ({type_})'
    ax1.plot(group['vns_iterations'], group['value'], label=label)
    ax2.plot(group['vns_iterations'], group['time'], linestyle='--', label=label)

# Personnalisation
ax1.set_title("Évolution de la valeur selon les itérations VNS")
ax1.set_ylabel("Valeur")
ax1.grid(True)
ax1.legend()

ax2.set_title("Évolution du temps selon les itérations VNS")
ax2.set_xlabel("VNS Iterations")
ax2.set_ylabel("Temps (s)")
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.show()


############################################### Temps #######################################################

df_time = pd.read_csv('benchmark/vns_gloutonne_vs_aleatoire_time.csv')

# Séparation des données par type d'algorithme
gloutonne_time = df_time[df_time['type'] == 'vns_gloutonne']
aleatoire_time = df_time[df_time['type'] == 'vns_aleatoire']

# Générer une palette de couleurs distinctes pour chaque fichier (utilisation de 'tab10')
unique_files = df_time['filename'].unique()
colors = plt.cm.get_cmap('tab10', len(unique_files))  # Utilisation de la palette 'tab10'

# Créer un dictionnaire qui associe chaque fichier à une couleur
file_colors = {filename: colors(i) for i, filename in enumerate(unique_files)}

# Créer un graphique pour l'évolution de la valeur en fonction du temps
plt.figure(figsize=(14, 7))

# Tracer les courbes pour chaque fichier
for filename in unique_files:
    # Sélectionner les données pour le fichier actuel
    gloutonne_file = gloutonne_time[gloutonne_time['filename'] == filename]
    aleatoire_file = aleatoire_time[aleatoire_time['filename'] == filename]
    
    # Obtenir la couleur associée au fichier
    color = file_colors[filename]

    # Tracer les courbes pour les deux types d'algorithmes
    plt.plot(gloutonne_file['time'], gloutonne_file['value'], label=f'VNS Gloutonne - {filename}', 
             marker='o', linestyle='-', color=color)
    plt.plot(aleatoire_file['time'], aleatoire_file['value'], label=f'VNS Aléatoire - {filename}', 
             marker='x', linestyle='--', color=color)

# Ajouter des labels et un titre
plt.xlabel('Temps')
plt.ylabel('Valeur')
plt.title('Évolution des valeurs des algorithmes VNS Gloutonne et VNS Aléatoire (par fichier)')

# Ajouter la légende et ajuster l'affichage
plt.legend()
plt.tight_layout()
plt.show()

################################################## Comparaison gloutonne et aleatoire #######################################################
df = pd.read_csv('benchmark/vnc_gloutonne_vs_aleatoire.csv')

grouped = df.groupby(['filename', 'type']).agg({'value': 'mean', 'time': 'mean'}).reset_index()

# Créer un graphique en barres pour les valeurs
plt.figure(figsize=(14, 7))

# Sépare les données par type d'algorithme
gloutonne = grouped[grouped['type'] == 'vns_gloutonne']
aleatoire = grouped[grouped['type'] == 'vns_aleatoire']

# Créer des indices pour les barres
x = range(len(gloutonne))
# Tracer les barres pour les valeurs
plt.bar(x, gloutonne['value'], width=0.4, label='VNS Gloutonne', color='blue', align='center')
plt.bar(x, aleatoire['value'], width=0.4, label='VNS Aléatoire', color='red', align='edge')

# Ajoute des labels et un titre
plt.xlabel('Fichier')
plt.ylabel('Valeur moyenne')
plt.title('Comparaison des algorithmes VNS Gloutonne vs VNS Aléatoire (temps_max : 5s, vns_iterations : 15000)')

plt.xticks(x, gloutonne['filename'], rotation=90)
plt.legend()
plt.tight_layout()  # Pour éviter les chevauchements de texte
plt.show()

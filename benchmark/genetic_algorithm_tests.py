import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import seaborn as sns

df = pd.read_csv('benchmark/genetic_algorithm_tests.csv')
df['pop_size'] = df['pop_size'].astype(int)
df['mutation_rate'] = df['mutation_rate'].astype(float)
df['value'] = df['value'].astype(float)
df['generations'] = df['generations'].astype(int)

# Moyenne pour éviter les doublons
grouped = df.groupby(['pop_size', 'mutation_rate']).agg({'value': 'mean'}).reset_index()

# Création des subplots

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 15))

#################################################### Valeur et Pop Size #######################################################

# 1. Valeur en fonction de pop_size
for rate in sorted(grouped['mutation_rate'].unique()):
    subset = grouped[grouped['mutation_rate'] == rate]
    ax1.plot(subset['pop_size'], subset['value'], label=f"Mutation {rate:.2f}")
ax1.set_title("Évolution de la valeur selon la taille de la population"+ df['filename'][0])
ax1.set_xlabel("Taille de population")
ax1.set_ylabel("Valeur")
ax1.grid(True)
ax1.legend()



################################################## Valeur et Mutation rate #######################################################
for pop in sorted(grouped['pop_size'].unique()):
    subset = grouped[grouped['pop_size'] == pop]
    ax2.plot(subset['mutation_rate'], subset['value'], label=f"Pop {pop}")
ax2.set_title("Évolution de la valeur selon le taux de mutation "+ df['filename'][0])
ax2.set_xlabel("Taux de mutation")
ax2.set_ylabel("Valeur")
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.show()


################################################## Valeur et Générations ######################################################

# Chargement des données
df = pd.read_csv('benchmark/genetic_algorithm_tests.csv')

# Regrouper les données par 'generations' et calculer la valeur moyenne
grouped_gen = df.groupby(['generations']).agg({'value': 'mean'}).reset_index()

# Créer un graphique en barres pour les valeurs par génération
plt.figure(figsize=(14, 7))

# Créer un indice pour les barres
x = range(len(grouped_gen))

# Tracer les barres pour les valeurs par génération
plt.bar(x, grouped_gen['value'], width=0.4, label='Valeur moyenne par génération', color='green', align='center')

# Ajouter des labels et un titre
plt.xlabel('Générations')
plt.ylabel('Valeur moyenne')
plt.title('Évolution de la valeur moyenne par génération '+ df['filename'][0])

# Ajouter les labels des générations sur l'axe X
plt.xticks(x, grouped_gen['generations'], rotation=90)

# Ajouter une légende
plt.legend()

# Ajuster l'agencement pour éviter les chevauchements de texte
plt.tight_layout()

# Afficher le graphique
plt.show()
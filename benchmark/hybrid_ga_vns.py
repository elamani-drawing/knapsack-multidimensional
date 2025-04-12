import pandas as pd
import matplotlib.pyplot as plt

# Chargement des données
df = pd.read_csv('benchmark/hybrid_ga_vns_test.csv')

# Moyenne des valeurs et du temps par fichier et type
grouped = df.groupby(['filename', 'type']).agg({
    'value': 'mean',
    'time': 'mean'
}).reset_index()

# Filtrer les données pour le type 'hybrid' uniquement
hybrid = grouped[grouped['type'] == 'hybrid']

# Créer un graphique pour les valeurs
plt.figure(figsize=(14, 7))

# Créer des indices pour les barres
x = range(len(hybrid))

# Tracer les barres pour les valeurs
plt.bar(x, hybrid['value'], width=0.4, label='Valeur moyenne', color='blue')

# Ajouter des labels et un titre
plt.xlabel('Fichier')
plt.ylabel('Valeur moyenne')
plt.title('Valeur moyenne pour l\'algorithme Hybrid GA ')

# Ajouter les ticks pour l'axe des x
plt.xticks(x, hybrid['filename'], rotation=90)

# Ajouter la légende
plt.legend()

# Ajuster la disposition pour éviter les chevauchements de texte
plt.tight_layout()

# Afficher le graphique des valeurs
plt.show()

# Créer un graphique pour les temps
plt.figure(figsize=(14, 7))

# Tracer les barres pour les temps
plt.bar(x, hybrid['time'], width=0.4, label='Temps moyen', color='red')

# Ajouter des labels et un titre
plt.xlabel('Fichier')
plt.ylabel('Temps moyen')
plt.title('Temps moyen pour l\'algorithme Hybrid GA')

# Ajouter les ticks pour l'axe des x
plt.xticks(x, hybrid['filename'], rotation=90)

# Ajouter la légende
plt.legend()

# Ajuster la disposition pour éviter les chevauchements de texte
plt.tight_layout()

# Afficher le graphique des temps
plt.show()

import pandas as pd

# Чтение CSV-файла
df = pd.read_csv('groups_30.csv')

# Замена русских букв на английские в столбце Degree
df['Degree'] = df['Degree'].replace({
    'Б': 'B',
    'С': 'C',
    'М': 'M'
})

# Сохранение обратно в CSV
df.to_csv('groups_30.csv', index=False, encoding='utf-8')

print("Замена завершена. Результат сохранен в groups_30.csv")

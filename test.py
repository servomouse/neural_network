# pip install pyarrow, pandas, pillow
import pandas as pd
# import pyarrow.parquet as pq
import json
from PIL import Image
from io import BytesIO
import base64

dataset_file = "D:\Work\Library\Datasets\MNIST\test-00000-of-00001.parquet"

# Read the parquet file:
df = pq.read_table(dataset_file).to_pandas()

# # import pandas as pd
# df = pd.read_parquet('example_fp.parquet', engine='fastparquet')

descriptions = []
for index, row in df.iterrows():

    for key, value in row.items():
        print(f"{key = }")
    exit()

    # Extract image data and description
    image_data = row['image_column']    # Replace with actual name
    description = row['description']    # Replace with actual name

    # Convert image data to an image and save as PNG
    image = Image.open(BytesIO(base64.b64decode(image_data)))
    image.save(f"image_{index}.png")

    # Collect descriptions
    descriptions.append({'index': index, 'description': description})

# Save all descriptions to a JSON file
with open("descriptions.json", 'w') as f:
    json.dump(descriptions, f, indent=4)

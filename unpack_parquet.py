# pip install pyarrow, pandas, pillow
import pandas as pd
import pyarrow.parquet as pq
import json
from PIL import Image
from io import BytesIO
import base64

dataset_folder = "../../Library/Datasets/MNIST"
dataset_file = "test-00000-of-00001.parquet"

# Read the parquet file:
df = pq.read_table(f"{dataset_folder}/{dataset_file}").to_pandas()

def add_padding(b64_string):
	missing_padding = 4 - len(b64_string) % 4
	if missing_padding > 0:
		b64_string += b'=' * missing_padding
	return b64_string

descriptions = []
for index, row in df.iterrows():

	# for key, value in row.items():
	#     print(f"{key = }")
	# exit()

	# Extract image data and description
	image_data = row['image']['bytes']    # Replace with actual name
	description = row['label']    # Replace with actual name

	# image_data = add_padding(image_data)
	# print(f"{image_data}")
	# exit()

	# # Convert image data to an image and save as PNG
	# image = Image.open(BytesIO(base64.b64decode(image_data)))
	# image.save(f"{dataset_folder}/image_{index}.png")
	with open(f"{dataset_folder}/images/{dataset_file}_{index}.png", 'wb') as f:
		f.write(image_data)

	# Collect descriptions
	descriptions.append({'index': index, 'description': description})

# Save all descriptions to a JSON file
with open(f"{dataset_folder}/descriptions.json", 'w') as f:
    json.dump(descriptions, f, indent=4)

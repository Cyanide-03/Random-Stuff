import requests

url = "http://192.168.1.10/api/scan"

response = requests.get(url, timeout=10)
response.raise_for_status()

scan = response.json()

print("Complete:", scan["scan_complete"])
print("Scan ID:", scan["scan_id"])
print("Best angle:", scan["best_angle"])

for measurement in scan["measurements"]:
    print(measurement["angle"], measurement["distance_cm"])
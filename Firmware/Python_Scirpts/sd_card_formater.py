import os 

pattern = bytearray(b'\xDE\xAD\xBE\xEF')  # Wzorzec do wyszukania (4 bajty 0xFF)
secotr_size = 512  # Rozmiar sektora w bajtach
secotr_number = 0  # Numer sektora do sprawdzenia

with open(r"\\.\PhysicalDrive1", "r+b") as sd_card:

    sd_card_data = sd_card.write(pattern*(secotr_size // len(pattern)))  # Odczytaj sektor danych
    sd_card.seek(secotr_number * secotr_size) 
    sector_data = sd_card.read(secotr_size) 



    
COVID-19 has become the name of fear for people of all over the world. It has been the reason for mass death for the last four years. To fight against the disease, it is very important to identify the affected people early, isolate them, and treat them. In this work, a system is created to detect COVID-19 at a very early stage. The wearable IoT device reads the temperature, heart rate, and oxygen saturation using DS18B20 (Temperature Sensor) and MAX30100 (Heart Rate and Pulse Oximeter Sensor). Firebase Real-Time database is used to store the data. The “COUGHVID crowdsourcing dataset” is used for cough recordings. MFCC (Mel-frequency cepstral coefficients) is used to extract features from the cough recordings. These features are used to train the machine learning models. Various machine learning algorithms (DTC, SVM, LR, RFC, and NBC) are used on the COVID-19 data (194 COVID-19-positive, 104 Healthy). Among them, DTC performed best with an accuracy of 91.03% and F1-score of 89.39% for detecting COVID-19. A website is used to record cough, show the IoT data and send an email if COVID-19 is detected. The work shows how digital devices can monitor and detect COVID-19.
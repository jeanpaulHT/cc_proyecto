
create table utec.taxi.data(
VendorID                   int,
lpep_pickup_datetime      string,
Lpep_dropoff_datetime     string,
Store_and_fwd_flag        string,
RateCodeID                 int,
Pickup_longitude         float,
Pickup_latitude          float,
Dropoff_longitude        float,
Dropoff_latitude         float,
Passenger_count            int,
Trip_distance            float,
Fare_amount              float,
Extra                    float,
MTA_tax                  float,
Tip_amount               float,
Tolls_amount             float,
Ehail_fee                float,
improvement_surcharge    float,
Total_amount             float,
Payment_type               int,
Trip_type                float,
 extra1                  float,
 extra2                  float
);

create or replace file format CSV_FORMAT_HIPO
  type = 'CSV'
  field_delimiter = ','
  skip_header = 1
  compression = none
  RECORD_DELIMITER = '\r\n'
  FIELD_OPTIONALLY_ENCLOSED_BY = '"'
  ERROR_ON_COLUMN_COUNT_MISMATCH = FALSE
 ;

--2.
CREATE OR REPLACE STAGE HIPO_LKT2
URL = 's3://datalake-cloud-utec/taxi/'
CREDENTIALS = (AWS_KEY_ID = 'AKIATAKRBWBCYKLYPAMC' AWS_SECRET_KEY = 'XEGTGtnVtkoiOOww4fg0dSF7N/UPcM4t2WUxlh2l')
file_format = CSV_FORMAT_HIPO;

COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-06.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-07.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-08.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-09.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-10.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-11.csv') FORCE = TRUE;
COPY INTO UTEC.TAXI.DATA
FROM @HIPO_LKT2 files=('green_tripdata_2015-12.csv') FORCE = TRUE;

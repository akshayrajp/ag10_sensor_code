import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
import constants as C

def write(climate_variable, value):

    bucket = C.influxdb_bucket
    org = C.influxdb_org
    token = C.influxdb_token
    url = C.influxdb_url

    client = influxdb_client.InfluxDBClient(
        url=url,
        token=token,
        org=org
    )

    write_api = client.write_api(write_options=SYNCHRONOUS)
    
    p = influxdb_client.Point("measurement").tag("location", "Hyderabad").field(
        climate_variable, value)
    write_api.write(bucket=bucket, org=org, record=p)

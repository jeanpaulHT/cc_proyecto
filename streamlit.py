# Import python packages
import streamlit as st
from snowflake.snowpark.context import get_active_session
from snowflake.snowpark.functions import col


# Write directly to the app
st.title("Example Streamlit App for streamlit :balloon:")
st.write(
    """This demo is used to test the connection between streamlit and snowflake data
        brought by the etl process in aws
    """
)

# Get the current credentials
session = get_active_session()



df = session.sql("select * from utec.taxi.data ")
df = df.select(
    col("PICKUP_LONGITUDE").alias("lon"),
    col("PICKUP_LATITUDE").alias("lat"),
    # Include other columns you need here
)

# Convert to pandas DataFrame
df_pandas = df.to_pandas()

st.write(f"the number of rows extracted in snow was {df.count():,}")

st.map(df)

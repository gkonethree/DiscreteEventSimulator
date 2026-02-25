# Running Instructions

First install required libraries using: \
```pip install requirements.txt```\\

Then modify parameters in [config.py](config.py) \
Set the number of users in [main.py](main.py). You may do a for loop if you want results for multiple users. \
Finally run \
```python3 main.py```



# To-Do
* [ ] Request retry mechanism
* [ ] Drop rate vs no of users
* [ ] Core utilization vs no of users
* [ ] Any other metrics out of curiosity (optional)

[metrics.py](metrics.py) is where the metrics are recorded. \
[webserver.py](webserver.py) simulates the webserver. Can add storing more metrics there.

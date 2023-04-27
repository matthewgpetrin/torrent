import csv
import math
import rtree
import numpy

# ON STARTUP - RUN THIS ONCE AND SAVE THE RETURN VALUE
# pass it a trnt (csv) file and the distance between points (in GPS)
# returns a 2d python list and an rtree object that can be searched
def read_trnt(trnt, dist):
    trnt_table = []
    with open(trnt, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            trnt_table.append([float(row[0]), 
                               float(row[1]), 
                               float(row[2])])
    
    trnt_rtree = rtree.index.Index()
    for i, row in enumerate(trnt_table):
        trnt_rtree.insert(i, (row[0] - (dist/2), row[1] - (dist/2), 
                              row[0] + (dist/2), row[1] + (dist/2)))
        
    return [trnt_table, trnt_rtree]


# SEARCH TABLE - RETURNS [nearest lat, nearest lon, angle] 
# you should always pass the same trnt data object to this
# this is just a much faster way of searching the coords
def scan_trnt(lat, lon, data):
    nearest = list(data[1].nearest((lat , lon , lat , lon), num_results=1))[0]
    return data[0][nearest]


# CONVERT TX ANGLE TO PHASE DELAYS - RETURNS [phase0, phase1, phase2, ...]
# be sure to adjust the tx angle by magnetometer data
def angle_to_phase(angle, diameter, Nantennas, frequency):
    pi = math.pi

    start_angle = pi + (pi / Nantennas)
    positions = [[diameter / 2 * math.cos(start_angle + (i * 2 * pi / Nantennas)),
                  diameter / 2 * math.sin(start_angle + (i * 2 * pi / Nantennas)),
                  0] for i in range(Nantennas + 1)]

    direction = [-math.cos(angle), -math.sin(angle), 0]

    taus = [numpy.dot(positions[i], direction) / 3e8 for i in range(len(positions))]
    
    phases = [math.radians(tau * 360 * frequency) for tau in taus]

    return phases


# ********* EXAMPLE USAGE *********
import time
start_time = time.time()

data = read_trnt('canavan.trnt', 0.000025)

angle = scan_trnt(40.7441,-74.0259751, data)[2]
        
print(angle)

print("--- %s seconds ---" % (time.time() - start_time))

print(angle_to_phase(90, .125, 8, 2.4e9))


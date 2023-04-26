import csv
import rtree

# ON STARTUP - RUN THIS ONCE AND SAVE THE RTREE
# pass it a trnt (csv) file and the distance between points (in GPS)
# returns a 2d python list and an rtree object that can be searched
def parse_trnt(trnt, dist):
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
def search_trnt(lat, lon, data):
    nearest = list(data[1].nearest((lat , lon , lat , lon), num_results=1))[0]
    return data[0][nearest]
    

# EXAMPLE USAGE
import time
start_time = time.time()


data = parse_trnt('canavan.trnt', 0.000025)

angle = search_trnt(40.744,-74.025975, data)[2]
        
print(angle)

print("--- %s seconds ---" % (time.time() - start_time))
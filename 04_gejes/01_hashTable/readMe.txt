Usage: ./badTable [-i input] [-o output]
for input data from file "input" and out it to file "output"
input should be like this:

integer
comand [args]
end

where integer - first (begining) size of table;
comand - one of this:
add (string key) (int value) - add a new value. It can resize the table.
get (string key) - out value from key 'key'
pop (string key) - remove value from key 'key'

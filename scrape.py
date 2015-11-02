#!/usr/bin/python2
from sys import stderr
import json
import re
import requests

def main():
    url = 'http://catfacts-api.appspot.com/api/facts?number=100'
    max_attempts = 100

    all_facts = []
    for i in range(0, max_attempts):
        stderr.write("{}\n".format(i))
        response = requests.get(url)
        jsonified = response.json()
        for fact in jsonified["facts"]:
            regex = re.compile(re.escape("cat"), re.IGNORECASE)
            new_fact = regex.sub("printer", fact)
            all_facts.append(new_fact)

    all_facts = list(set(all_facts))
    json_facts = json.dumps(all_facts)

    outfile = open("all_facts.txt", "w")
    outfile.write(json_facts)
    outfile.close()

if __name__ == '__main__':
    main()

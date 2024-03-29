import json
import xml.etree.ElementTree as ET

data = {
    "XML": {
        "standard release date": 1998,
        "example libraries": [
            {
                "C++": [
                    "rapidXML",
                    "tinyXML2"
                ]
            },
            {
                "Python": [
                    "xml.etree.ElementTree"
                ]
            },
            {
                "Java": [
                    "DOM Parser",
                    "SAX Parser"
                ]
            }
        ],
        "supports bool": "true",
        "% of Stack Overflow questions in 2019": 0.79
    },
    "JSON": {
        "standard release date": 2017,
        "example libraries": [
            {
                "C++": [
                    "rapidJSON"
                ]
            },
            {
                "Python": [
                    "json"
                ]
            },
            {
                "Java": [
                    "org.json",
                    "Google GSON"
                ]
            },
            {
                "Fortran": "null"
            }
        ],
        "supports bool": "false",
        "% of Stack Overflow questions in 2019": 1.45
    }
}

with open('data.json', 'w') as out:
    json.dump(data, out, indent=4)

with open('data.json') as f:
    jsonObject = json.load(f)

Java = jsonObject['XML']['example libraries'][2]['Java']
for parser in Java:
    print(f"Java parser: {parser}")

# root = ET.Element("XML")

# ET.SubElement(root, "standard_release_date").text = str(jsonObject['XML']['standard release date'])
# # ET.SubElement(root, "example_libraries").text = str(jsonObject['XML']['example libraries'])
# ET.SubElement(root, "supports_bool").text = str(jsonObject['XML']['supports bool'])
# ET.SubElement(root, "percent_of_Stack_Overflow_questions_in_2019").text = str(jsonObject['XML']['% of Stack Overflow questions in 2019'])

# a = ET.ElementTree(root)

# a.write("data.xml")
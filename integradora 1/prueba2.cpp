#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

string leerArchivos(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Error while opening the file: " << fileName << endl;
    }

    string content;
    string line;

    while (getline(file, line))
    {
        content += line + '\n';
    }

    return content;
}

// devuelve las posiciones de ocurrencias
vector<int> z_function(const string &texto)
{
    int n = texto.length();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++)
    {
        if (i < r)
        {
            z[i] = min(r - i, z[i - l]);
        }

        while (i + z[i] < n && texto[z[i]] == texto[i + z[i]])
        {
            z[i]++;
        }

        if (i + z[i] > r)
        {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

void buscarOcurrencias(const string &transmission, const string &mcode, const string &transmissionName, ofstream &outputFile)
{
    string pattern;
    istringstream mcodeStream(mcode);
    while (getline(mcodeStream, pattern))
    {
        // string combinado con el patron$transmission
        string combined = pattern + "$" + transmission;
        // aplicar z function
        vector<int> positions;
        vector<int> z = z_function(combined);

        // recorrer las posiciones que devuelve y encontrar las posiciones donde hay coincidencia completa
        for (int i = pattern.size() + 1; i < z.size(); ++i)
        {
            // si se encuentra parte del patron
            if (z[i] == pattern.size())
            {
                // posicion actual
                int position = i - pattern.size() - 1;
                positions.push_back(position);
            }
        }

        // si no existen posiciones se sale de este if
        if (!positions.empty())
        {
            // output de checking.txt
            outputFile << "Código: " << pattern << endl;
            outputFile << transmissionName << " ==> " << positions.size() << " veces" << endl;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                outputFile << positions[i];
                if (i != positions.size() - 1)
                {
                    outputFile << ", ";
                }
            }
            outputFile << endl;
        }
    }
}

int main()
{
    // l
    string transmission1 = leerArchivos("transmission1.txt");
    string transmission2 = leerArchivos("transmission2.txt");
    string transmission3 = leerArchivos("transmission3.txt");
    string mcode = leerArchivos("mcode.txt");

    ofstream outputFile("checking.txt");
    buscarOcurrencias(transmission1, mcode, "transmission1.txt", outputFile);
    buscarOcurrencias(transmission2, mcode, "transmission2.txt", outputFile);
    buscarOcurrencias(transmission3, mcode, "transmission3.txt", outputFile);

    outputFile.close();

    return 0;
}

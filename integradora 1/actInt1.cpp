#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Evidencia 1

// José Alberto Ruiz Rodríguez A00836129
// Fernanda Vásquez A00837146
// Función para transformar la cadena añadiendo caracteres '#' entre cada letra
string transformarCadena(const string &s)
{
    string t = "#";
    for (char c : s)
    {
        t += c;
        t += '#';
    }
    return t;
}

// Función para encontrar el palíndromo más largo usando el algoritmo de Manacher

// complejidad O(n*m)
string manacher(const string &s, int &start, int &maxLen)
{
    // Transformar la cadena original añadiendo '#'
    string t = transformarCadena(s);
    int n = t.size();

    vector<int> P(n, 0); // Radios de los palíndromos
    int C = 0, R = 0;    // Centro y límite derecho del palíndromo más largo encontrado hasta ahora

    for (int i = 0; i < n; ++i)
    {
        int mirror = 2 * C - i;
        if (i < R)
        {
            P[i] = min(R - i, P[mirror]);
        }

        // Expandir el palíndromo centrado en i
        while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 && t[i + P[i] + 1] == t[i - P[i] - 1])
        {
            P[i]++;
        }

        // Actualizar el centro y el radio si hemos encontrado un palíndromo más grande
        if (i + P[i] > R)
        {
            C = i;
            R = i + P[i];
        }
    }

    maxLen = 0;
    int centerIndex = 0;
    for (int i = 0; i < n; ++i)
    {
        if (P[i] > maxLen)
        {
            maxLen = P[i];
            centerIndex = i;
        }
    }

    start = (centerIndex - maxLen) / 2; // Posición de inicio del palíndromo más largo en la cadena original
    return s.substr(start, maxLen);     // Palíndromo más largo
}

// Leer el contenido de un archivo
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

// Devuelve las posiciones de ocurrencias usando la función Z
// complejidad O(n+m)
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

// Función para buscar la subsecuencia más frecuente con un carácter eliminado en los tres archivos
// complejidad O(n^2)
void buscarSubsecuencia(const string &pattern, const vector<string> &transmissions, ofstream &outputFile)
{
    int maxCount = 0;
    string mostFrequentSubsequence;
    string transmissionWithMostOccurrences;

    // Generar todas las subsecuencias posibles eliminando un carácter
    for (int i = 0; i < pattern.size(); ++i)
    {
        string subsequence = pattern.substr(0, i) + pattern.substr(i + 1);

        // Buscar subsecuencia en todas las transmisiones
        int highestCountInTransmission = 0;
        string currentTransmission;

        for (int j = 0; j < transmissions.size(); ++j)
        {
            string combined = subsequence + "$" + transmissions[j];
            vector<int> z = z_function(combined);

            int count = 0;
            for (int k = subsequence.size() + 1; k < z.size(); ++k)
            {
                if (z[k] == subsequence.size())
                {
                    count++;
                }
            }

            // si la subsecuencia se encuentra más veces en esta transmisión específica
            if (count > highestCountInTransmission)
            {
                // actualizarlo
                highestCountInTransmission = count;
                currentTransmission = "Transmission" + to_string(j + 1) + ".txt";
            }
        }

        //  la subsecuencia más frecuente considerando las transmisiones
        if (highestCountInTransmission > maxCount)
        {
            // actualizar con el maxCount
            maxCount = highestCountInTransmission;
            mostFrequentSubsequence = subsequence;
            transmissionWithMostOccurrences = currentTransmission;
        }
    }

    // escribirlo en el output
    if (!mostFrequentSubsequence.empty())
    {
        outputFile << "La subsecuencia más encontrada es: " << mostFrequentSubsequence
                   << " con " << maxCount << " veces en el archivo " << transmissionWithMostOccurrences << endl;
    }
}

// funcion para buscar las ocurrencias de un patrón en una transmisión
// O(n + m) n es la longitud de transmición y m la longitud de el patron
void buscarOcurrencias(const vector<string> &transmissions, const string &mcode, ofstream &outputFile)
{
    string pattern;
    istringstream mcodeStream(mcode);

    while (getline(mcodeStream, pattern))
    {
        outputFile << "Código: " << pattern << endl;

        // dentro del array de files
        for (size_t j = 0; j < transmissions.size(); ++j)
        {
            // combinar el string del patron$texto_de_transmision
            string combined = pattern + "$" + transmissions[j];
            vector<int> z = z_function(combined);

            vector<int> positions;
            for (int i = pattern.size() + 1; i < z.size(); ++i)
            {
                if (z[i] == pattern.size())
                {
                    int position = i - pattern.size() - 1;
                    // hacer push en el vector de posiciones
                    positions.push_back(position);
                }
            }

            // escribir en el archivo si hay ocurrencias
            if (!positions.empty())
            {
                outputFile << "Transmission" << j + 1 << ".txt ==> " << positions.size() << " veces" << endl;
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
            // si no hay
            else
            {
                outputFile << "Transmission" << j + 1 << ".txt ==> No encontrado" << endl;
            }
        }

        //  la subsecuencia más común con un carácter eliminado en las tres transmisiones
        buscarSubsecuencia(pattern, transmissions, outputFile);
        outputFile << "--------------" << endl;
    }
}

// Función para encontrar el palíndromo más largo y dividirlo en partes
void procesarPalindromo(const string &transmission, const string &transmissionName, ofstream &outputFile)
{
    int start = 0, maxLen = 0;
    string palindromo = manacher(transmission, start, maxLen);

    // Dividir el mensaje en left, middle, right
    string left = transmission.substr(0, start);
    string middle = palindromo;
    string right = transmission.substr(start + maxLen);

    outputFile << "Palíndromo más grande:\n";
    outputFile << transmissionName << " ==> Posición: " << start << "\n";
    outputFile << middle << "\n";
    outputFile << "----\n";
}

// complejidad o(n*m)
string longestCommonSubstring(const string &s1, const string &s2)
{
    int n = s1.length();
    int m = s2.length();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    int length = 0;
    int endPos = 0;

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > length)
                {
                    length = dp[i][j];
                    endPos = i - 1;
                }
            }
        }
    }

    return s1.substr(endPos - length + 1, length);
}

// Función para analizar las transmisiones en pares
void procesarSimilitudes(const string &t1, const string &t2, const string &t3, ofstream &outputFile)
{
    string substringT1T2 = longestCommonSubstring(t1, t2);
    string substringT1T3 = longestCommonSubstring(t1, t3);
    string substringT2T3 = longestCommonSubstring(t2, t3);

    outputFile << "========================\n";
    outputFile << "Los Substring más largos son:\n";
    outputFile << "T1-T2 ==> " << substringT1T2 << endl;
    outputFile << "T1-T3 ==> " << substringT1T3 << endl;
    outputFile << "T2-T3 ==> " << substringT2T3 << endl;
    outputFile << "========================\n";
}

int main()
{
    // Leer transmisiones y códigos maliciosos
    string transmission1 = leerArchivos("transmission1.txt");
    string transmission2 = leerArchivos("transmission2.txt");
    string transmission3 = leerArchivos("transmission3.txt");
    string mcode = leerArchivos("mcode.txt");

    vector<string> transmissions = {transmission1, transmission2, transmission3};

    // Archivo de salida
    ofstream outputFile("checking.txt");

    buscarOcurrencias(transmissions, mcode, outputFile);

    procesarPalindromo(transmission1, "transmission1.txt", outputFile);
    procesarPalindromo(transmission2, "transmission2.txt", outputFile);
    procesarPalindromo(transmission3, "transmission3.txt", outputFile);

    procesarSimilitudes(transmission1, transmission2, transmission3, outputFile);

    // Cerrar archivo de salida
    outputFile.close();

    return 0;
}

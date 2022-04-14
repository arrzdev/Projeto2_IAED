"""
Script de testagem automática do primeiro projeto de IAED.

Utilização:

1 - Colocar o script na pasta onde se encontra o vosso projeto compilado.
2 - Colocar a pasta de testes publicos que transferem do fénix na pasta do vosso projeto,
    garantindo que o nome é exatamente "tests" e que o único conteúdo são os ficheiros .in e .out.
3 - Abrir o terminal.
4 - $ sudo python3 tester.py ./{proj}, 
    onde {proj} é o nome do ficheiro binário proveniente da compilação.


Output:
Quando o vosso programa faz o output correto, 
verão uma mensagem que confirma que passaram no teste.

Caso haja alguma diferença entre o output correto e o output do vosso programa, 
o script imprimirá um erro.

Convém sempre verificar manualmente as diferenças, e comparar o input com o output do teste, 
visto que a ferramenta que o script utiliza (diff), não é perfeita, e retorna um output difícil
de interpretar.

"""

import subprocess
import os
from pathlib import Path

project_name = 'proj1'

project_path = Path(__file__).absolute().parent
public_tests_folder = str(project_path) + '/proj2-public-tests/'
proj1_mooshak_tests_folder = str(project_path) + '/proj1-mooshak-tests/'

def run_public_tests():
    files_in = []
    files_out = []
    
    for f in os.listdir(public_tests_folder):
        if os.path.isfile(os.path.join(public_tests_folder, f)):
            if str(f)[-2:] == 'in':
                files_in.append(f)
            elif str(f)[-3:] == 'out':
                files_out.append(f)
    
    files_in = sorted(files_in)
    files_out = sorted(files_out)
    
    if len(files_out) != len(files_in):
        raise RuntimeError("ERRO - Testes inválidos")
    
    TEST_NR = len(files_in)
    
    passes = 0
    fails = 0
    
    for i in range(len(files_in)):
        test_path = public_tests_folder + files_in[i]
        check_path = public_tests_folder + files_out[i]
    
        output_path = public_tests_folder + f'output{(i+1):02d}.result'
    
        input = open(test_path)
        output = open(output_path, 'w')
    
        subprocess.run(
            [f"./{project_name}"], stdin=input, stdout=output)
        diff = subprocess.run(
            ["diff", check_path, output_path], stdout=subprocess.PIPE)
    
        if diff.returncode != 0:
            print(f"\nFAILED - Teste {(i+1):02d}\n")
            print(diff.stdout.decode("utf-8"))
            fails += 1
        else:
            print(f"OK - Teste {(i+1):02d}")
            passes += 1
    
    
    return f"""================================
RESULTADOS PUBLICOS:

Testes executados: {TEST_NR}

Passou: {passes}
Falhou: {fails}
================================
"""

def run_mooshak_tests():
    paths_in = []
    paths_out = []
    
    for dir in os.listdir(proj1_mooshak_tests_folder):
        if(os.path.isdir(os.path.join(proj1_mooshak_tests_folder, dir))):
            TEST = str(dir)
            paths_in.append(f'{dir}/input')
            paths_out.append(f'{dir}/output')
    
    paths_in = sorted(paths_in)
    paths_out = sorted(paths_out)
    
    if len(paths_in) != len(paths_out):
        raise RuntimeError("ERRO - Testes inválidos")
    
    fails = passes = 0
    
    for i in range(len(paths_in)):
        test_path = proj1_mooshak_tests_folder + paths_in[i]
        check_path = proj1_mooshak_tests_folder + paths_out[i]
    
        output_path = proj1_mooshak_tests_folder + paths_in[i].split('/')[0] + f'/result'
    
        input = open(test_path)
        output = open(output_path, 'w')
    
        subprocess.run(
            [f"./{project_name}"], stdin=input, stdout=output)
        diff = subprocess.run(
            ["diff", check_path, output_path], stdout=subprocess.PIPE)
    
        if diff.returncode != 0:
            print(f"\nFAILED - Teste {(i+1):02d}\n")
            print(diff.stdout.decode("utf-8"))
            fails += 1
        else:
            print(f"OK - Teste {(i+1):02d}")
            passes += 1
    
    
    return f"""================================
RESULTADOS MOOSHAK (1 PROJETO):

Testes executados: {len(paths_in)}

Passou: {passes}
Falhou: {fails}
================================
"""

mooshak_info = run_mooshak_tests()
public_info = run_public_tests()

print(f"\nINFO:")

print(mooshak_info)
print(public_info)
import kociemba 
import sys
import json

def solveCube(cubeState):
    try:
        solution=kociemba.solve(cubeState)
        return solution
    except Exception:
        return -1
    
if __name__ == "__main__":
    cubeState = sys.argv[1]
    solution = solveCube(cubeState)

    if (solution==-1):
        print(json.dumps({"Error": "Invalid Cube State"}))
    else:
        print(json.dumps({"Solution": solution}))

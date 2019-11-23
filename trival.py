import os
import traceback

# Process solutions afterwards to determine the non-trival ones.

# Write the solutions to a file. Read the file and for each
# solution, generate the trival solutions and add them to a hash
# table, marking the trival ones. After processing all the
# solutions, loop through the table and output the non-trival ones
# and the number of them.

# Remove the solutions found by rotating or flipping a solution.

# Found solution 9356
# elapsed time 14.55 sec
# 6662222444
# 6869952741
# 8899555741
# 8c9bb57731
# 8ccbaaa731
# ccbbaa3331

# flip left and right:

# 4442222666
# 1472599686
# 1475559988
# 13775bb9c8
# 137aaabcc8
# 1333aabbcc

# flip top and bottom:

# ccbbaa3331
# 8ccbaaa731
# 8c9bb57731
# 8899555741
# 6869952741
# 6662222444

# flip left and right (rotated original):

# 1333aabbcc
# 137aaabcc8
# 13775bb9c8
# 1475559988
# 1472599686
# 4442222666

# Add this to the hash table:

# 6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331 0
# 1333aabbcc137aaabcc813775bb9c8147555998814725996864442222666 t


# There are exactly 2339 solutions, excluding trivial variations
# obtained by rotation and reflection of the whole rectangle, but
# including rotation and reflection of a subset of pentominoes (which
# sometimes provides an additional solution in a simple way).

def main():
    trival = 0
    nontrival = 0
    filename = 'solution.out'
    solutions = read_solutions(filename)
    print("{} total solutions".format(len(solutions)))
    table = process_solutions(solutions)
    
    for k,v in table.items():
        if v == 't':
            trival += 1
        else:
            nontrival += 1

    print("{} trival solutions".format(trival))
    print("{} non-trival solutions".format(nontrival))
    print("")

    num = 1
    for k, v in table.items():
        if v == 0:
            solution = unflatten(k)
            print("{}:".format(num))
            print_solution(solution)
            print("")
            num += 1


def read_solutions(filename):
    if not os.path.exists(filename):
        raise Exception("File is not found: {}".format(filename))
    collect = 0
    solutions = []
    solution = []
    with open(filename, 'r') as fh:
        for line in fh:
            line = line.strip()
            if collect:
                if len(line) != 10:
                    raise Exception("solution line is not 10 characters")
                solution.append(line)
                if len(solution) == 6:
                    solutions.append(solution)
                    collect = 0
                    solution = []
            elif line.startswith('elapsed'):
                collect = 1

    return solutions


def process_solutions(solutions):
    table = {}
    for solution in solutions:
        flat = flatten(solution)
        if not flat in table:
            table[flat] = 0

        # flip left and right:
        sol = flip_left_right(solution)
        flat = flatten(sol)
        if not flat in table:
            table[flat] = 't'
        # else:
        #     print("already found: {}".format(flat))
        sol = flip_top_bottom(solution)
        flat = flatten(sol)
        if not flat in table:
          table[flat] = 't'
        # else:
        #     print("already found: {}".format(flat))

        sol = flip_left_right(sol)
        flat = flatten(sol)
        if not flat in table:
          table[flat] = 't'
        # else:
        #     print("already found: {}".format(flat))
    return table


def flip_left_right(solution):
    sol = []
    for line in solution:
        sol.append(line[::-1])
    return sol


def flip_top_bottom(solution):
    sol = solution[:]
    sol.reverse()
    return sol


def flatten(solution):
    return ''.join(solution)


def unflatten(flat):
    solution = []
    start = 0
    for i in range(0,60,10):
        solution.append(flat[i:i+10])
    return solution


def print_solution(solution):
    for line in solution:
        print(line)


if __name__ == "__main__":
    # args = parse_command_line(sys.argv)
    try:
        main()
    except KeyboardInterrupt:
        print("\nStopping")
    except Exception as ex:
        print(traceback.format_exc())

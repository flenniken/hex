import os
import unittest
from tempfile import mkstemp
from trival import flatten, unflatten, print_solution, read_solutions, \
    process_solutions, flip_left_right, flip_top_bottom

class TestTrival(unittest.TestCase):

  def test_flatten(self):
      flat = '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331'
      solution = unflatten(flat)
      self.assertEqual(len(solution), 6)
      self.assertEqual(solution[0], '6662222444')
      self.assertEqual(solution[1], '6869952741')
      self.assertEqual(solution[2], '8899555741')
      self.assertEqual(solution[3], '8c9bb57731')
      self.assertEqual(solution[4], '8ccbaaa731')
      self.assertEqual(solution[5], 'ccbbaa3331')
      self.assertEqual(flatten(solution), flat)

  def test_print_solution(self):
      flat = '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331'
      solution = unflatten(flat)
      # print('\n')
      # print_solution(solution)
      # print('\n')

  def test_flip_left_right(self):
      flat = '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331'
      solution = unflatten(flat)
      flipped = flip_left_right(solution)
      # print('\n')
      # print_solution(flipped)
      # print('\n')
      ff = flatten(flipped)
      self.assertEqual(ff, '44422226661472599686147555998813775bb9c8137aaabcc81333aabbcc')

  def test_flip_top_bottom(self):
      flat = '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331'
      solution = unflatten(flat)
      flipped = flip_top_bottom(solution)
      # print('\n')
      # print_solution(flipped)
      # print('\n')
      ff = flatten(flipped)
      # print(ff)
      self.assertEqual(ff, 'ccbbaa33318ccbaaa7318c9bb57731889955574168699527416662222444')

  def test_read_solutions(self):
      content = """\
Found solution 9356
elapsed time 14.47 sec
6662222444
6869952741
8899555741
8c9bb57731
8ccbaaa731
ccbbaa3331
"""
      fd, temp_filename = mkstemp()
      with open(temp_filename, 'w') as fh:
          fh.write(content)
      solutions = read_solutions(temp_filename)
      os.close(fd)
      os.remove(temp_filename)

      self.assertEqual(len(solutions), 1)
      # print('\n')
      # print_solution(solutions[0])
      # print('\n')
      flat = flatten(solutions[0])
      # print(flat)
      self.assertEqual(flat, '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331')

      table = process_solutions(solutions)
      # for k,v in table.items():
      #     print("{} = {}\n".format(k,v))
      self.assertEqual(len(table), 4)
      trival = nontrival = 0
      for k,v in table.items():
          if v == 't':
              trival += 1
          else:
              nontrival += 1
      self.assertEqual(trival, 3)
      self.assertEqual(nontrival, 1)
              

  def test_read_solutions2(self):
      content = """\
Found solution 9355
elapsed time 14.45 sec
22227aaabb
287777aab1
8866699bb1
8c64659931
8cc4555931
cc44453331

Found solution 9356
elapsed time 14.47 sec
6662222444
6869952741
8899555741
8c9bb57731
8ccbaaa731
ccbbaa3331
"""
      fd, temp_filename = mkstemp()
      with open(temp_filename, 'w') as fh:
          fh.write(content)
      solutions = read_solutions(temp_filename)
      os.close(fd)
      os.remove(temp_filename)

      self.assertEqual(len(solutions), 2)
      # print('\n')
      # print_solution(solutions[0])
      # print('\n')
      flat = flatten(solutions[0])
      # print(flat)
      self.assertEqual(flat, '22227aaabb287777aab18866699bb18c646599318cc4555931cc44453331')

      flat = flatten(solutions[1])
      self.assertEqual(flat, '6662222444686995274188995557418c9bb577318ccbaaa731ccbbaa3331')


      table = process_solutions(solutions)
      # for k,v in table.items():
      #     print("{} = {}\n".format(k,v))

      self.assertEqual(len(table), 8)
      trival = nontrival = 0
      for k,v in table.items():
          if v == 't':
              trival += 1
          else:
              nontrival += 1
      self.assertEqual(trival, 6)
      self.assertEqual(nontrival, 2)


  def test_read_solutions3(self):
      solutions = read_solutions('solution.out')
      self.assertEqual(len(solutions), 9356)

      table = process_solutions(solutions)
      # for k,v in table.items():
      #     print("{} = {}\n".format(k,v))

      trival = nontrival = 0
      for k, v in table.items():
          if v == 't':
              trival += 1
          else:
              nontrival += 1
      # print("trival = {}".format(trival))
      # print("nontrival = {}".format(nontrival))
      self.assertEqual(trival, 7017)
      self.assertEqual(nontrival, 2339)


if __name__ == '__main__':
  unittest.main()

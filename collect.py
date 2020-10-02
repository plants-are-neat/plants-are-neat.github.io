from git import Repo
import matplotlib.pyplot as plt
import os
import shutil

A = [1, 2, 3, 4, 5, 6, 7];
B = [11, 22 ,33 ,44, 55, 66, 77];
plt.plot(A,B);
#plt.show();
plt.savefig('Graph1.png');
os.rename('/Users/18322/plants-are-neat/plants-are-neat.github.io/Graph1.png', '/Users/18322/plants-are-neat/plants-are-neat.github.io/data/Graph1.png');
git push
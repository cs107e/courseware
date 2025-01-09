# Assignment 0 - Assignment workflow, Lab, and background

This assignment walks students through a general workflow for assignments, and
to test their background knowledge of some preliminary topics: electricity,
boolean algebra, git, and the command line.

## Core:

We don't assign grades for this assignment, but here are some brief notes on the background questions.

1. What is the result of performing a bitwise OR of the following two hexadecimal numbers (include the result in decimal): 0x0f OR 0xff. Now left shift that number by 24; what is the result in hexadecimal? Assume numbers are 32-bit.

**Answer:**

0x0f | 0xff = 0b00001111 | 0b11111111 = 0b11111111 = 0xff = 255

---

2. How many milliamperes of current will flow through a 1.5K resistor connected to 3V on one end and 0V (Ground) on the other end?

**Answer:**

User **V=IR**.

3v = I \* (1.5k)

3v / 1500 Ohm = **I** = 0.002A = 2mA

---

3. Find a file in the directory /etc on your computer; tell us the name of the file and the number of bytes contained in that file. How many files and directories are contained in the first level of directory /etc? Include the commands you used to answer these questions.

**Answer:**
Answers to this question may vary, but they should just be using a combination of `ls` and `wc`, or some other simple variant. If they provide a solution that uses anything other than the command line, we should given them an explanation using their chosen commands.

---

4. Find the path to the include/ directory in your cs107e.github.io repo and type the absolute path to this folder, and the relative path to the folder using the CS107E environment variable you created during your environment setup. Remember this folder location! This is where all your assignment header files are located.

**Answer:**
We want students to remember this file location for reference in assignments.

Absolute path: /user/blah blah/cs107e_home/cs107e.github.io/
Relative path: $CS107E/include

---

5. What editor do you plan on using for the class? What is your plan on learning how to best use your editor? (i.e. for people using vim, you can run the vimtutor command from your terminal). Tell us a bit about how you customized your editor!

**Answer:**

We really want to stress that students should be focusing on learning their tools. A big part of that is getting comfortable with an editor chosen by the student. All students should be comfortable using and configuring vim or emacs on their machines. This is critical for effective use of the command line. Students don't need to be using vim or emacs as their regular IDE, but they should at least be comfortable with them as tools.

Students should not be using something like notepad for editting files. Ideally they'd be using something like VSCode, Sublime Text, or Atom. QtCreator and XCode are also not great IDE choices for this class since our projects aren't really built for those environments.

---

6. In a version control system like git, what is a "commit"? What is a "branch"? If you're unsure, refer to the git guide on the course website.

**Answer:**

Students demonstrate their knowledge of Git here. They should have a good handle on what a commit and branch is. They should not be copying and pasting answers to these questions.

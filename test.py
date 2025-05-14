# Generating a pipeline of 6000 'ls' commands separated by pipes
num_commands = 9000
command = "ls"
pipeline = " | ".join([command] * num_commands)

# Writing the pipeline to a file
with open("pipeline_test.txt", "w") as f:
    f.write(pipeline)
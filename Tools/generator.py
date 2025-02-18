import tkinter as tk
from tkinter import filedialog, messagebox, ttk

def generate_files():
    module_name = entry_module.get()
    functions = []

    if not module_name:
        messagebox.showerror("Błąd", "Podaj nazwę modułu!")
        return

    for i in range(10):
        func_name = entries_functions[i].get()
        return_type = return_type_vars[i].get()
        args = [(arg_type_vars[i][j].get(), arg_name_entries[i][j].get()) for j in range(5) if arg_name_entries[i][j].get()]
        is_static = static_vars[i].get()

        if func_name:
            functions.append((func_name, return_type, args, is_static))

    header_guard = module_name.upper() + "_H"
    header_content = f"""
#ifndef {header_guard}
#define {header_guard}

"""

    source_content = f"""
#include "{module_name}.h"

"""

    static_functions = []
    global_functions = []

    for func_name, return_type, args, is_static in functions:
        args_str = ", ".join([f"{arg_type} {arg_name}" for arg_type, arg_name in args]) if args else "void"
        function_name = func_name if is_static else f"{module_name}_{func_name}"

        doxy_params = "\n".join([f" * @param  {arg_name} {arg_type} - Description." for arg_type, arg_name in args]) if args else " * @param  None"
        doxy_comment = f"""/** 
 * @brief  Brief description of {function_name}.
{doxy_params}
 * @retval {return_type} Description of return value.
 */

"""

        function_declaration = f"{return_type} {function_name}({args_str});"
        function_definition = f"{doxy_comment}{'static ' if is_static else ''}{return_type} {function_name}({args_str})\n{{\n\n}}\n\n"

        if is_static:
            static_functions.append(function_definition)
        else:
            header_content += function_declaration + "\n"
            global_functions.append(function_definition)

    header_content += "\n#endif"
    source_content += "\n\n".join(static_functions) + "\n\n" + "\n\n".join(global_functions)

    with open(f"{module_name}.h", "w") as h_file:
        h_file.write(header_content)

    with open(f"{module_name}.c", "w") as c_file:
        c_file.write(source_content)

    messagebox.showinfo("Sukces", "Pliki zostały wygenerowane!")
    root.destroy()

# Tworzenie GUI
root = tk.Tk()
root.title("Generator Plików C")
root.configure(bg='#2E2E2E')

frame = tk.Frame(root, padx=30, pady=30, bg='#2E2E2E')
frame.pack()

tk.Label(frame, text="Nazwa modułu:", font=("Arial", 14), bg='#2E2E2E', fg='white').grid(row=0, column=0, sticky='w', padx=10, pady=10)
entry_module = tk.Entry(frame, font=("Arial", 14), width=30)
entry_module.grid(row=0, column=1, padx=10, pady=10, columnspan=2)

entries_functions = []
return_type_vars = []
arg_type_vars = []
arg_name_entries = []
static_vars = []

types = ["void", "void*", "uint8_t", "uint16_t", "uint32_t", "uint64_t", "uint8_t*", "uint16_t*", "uint32_t*", "uint64_t*", "custom"]

tk.Label(frame, text="Static", bg='#2E2E2E', fg='white').grid(row=1, column=0, padx=10)
tk.Label(frame, text="Nazwa funkcji", bg='#2E2E2E', fg='white').grid(row=1, column=1, padx=10)
tk.Label(frame, text="Typ zwracany", bg='#2E2E2E', fg='white').grid(row=1, column=2, padx=10)
tk.Label(frame, text="Argumenty", bg='#2E2E2E', fg='white').grid(row=1, column=3, columnspan=10, padx=10)
tk.Label(frame, text="Typ argumentu", bg='#2E2E2E', fg='white').grid(row=1, column=3, padx=5)
tk.Label(frame, text="Nazwa zmiennej", bg='#2E2E2E', fg='white').grid(row=1, column=4, padx=5)

for i in range(10):
    static_var = tk.BooleanVar()
    static_check = tk.Checkbutton(frame, variable=static_var, bg='#2E2E2E')
    static_check.grid(row=i+2, column=0, padx=10)
    static_vars.append(static_var)

    entry_function = tk.Entry(frame)
    entry_function.grid(row=i+2, column=1, padx=10)
    entries_functions.append(entry_function)

    return_type_var = tk.StringVar()
    return_type_dropdown = ttk.Combobox(frame, textvariable=return_type_var, values=types, state="readonly")
    return_type_dropdown.grid(row=i+2, column=2, padx=10)
    return_type_vars.append(return_type_var)

    arg_types_row = []
    arg_names_row = []

    for j in range(5):
        arg_type_var = tk.StringVar()
        arg_type_dropdown = ttk.Combobox(frame, textvariable=arg_type_var, values=types, state="readonly", width=10)
        arg_type_dropdown.grid(row=i+2, column=3 + 2 * j, padx=5)
        arg_types_row.append(arg_type_var)

        entry_arg_name = tk.Entry(frame, width=10)
        entry_arg_name.grid(row=i+2, column=4 + 2 * j, padx=5)
        arg_names_row.append(entry_arg_name)

    arg_type_vars.append(arg_types_row)
    arg_name_entries.append(arg_names_row)

tk.Button(frame, text="Generuj", command=generate_files, width=20, height=2).grid(row=12, columnspan=10, pady=20)

root.mainloop()

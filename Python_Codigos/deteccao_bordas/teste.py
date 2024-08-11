import torch

if torch.cuda.is_available():
    print("GPU disponível:", torch.cuda.get_device_name(0))
else:
    print("Nenhuma GPU disponível")

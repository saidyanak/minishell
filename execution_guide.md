/* 
🎯 MINISHELL EXECUTION ARCHITECTURE GUIDE
=======================================

1️⃣ SINGLE COMMAND EXECUTION (Şu anki durum)
├── buildin mi kontrol et
├── buildin ise: ft_build_in() çağır
└── değilse: external command çalıştır
    ├── PATH'te ara
    ├── fork() yap
    ├── child'da execve() çağır
    └── parent'ta wait() yap

2️⃣ MULTIPLE COMMAND EXECUTION (Gelecek)
├── pipe var mı kontrol et
├── her komut için ayrı process
├── pipe setup yap
└── paralel execution

3️⃣ REDIRECTION HANDLING
├── input redirection (<)
├── output redirection (>)
├── append redirection (>>)
└── heredoc (<<)
*/

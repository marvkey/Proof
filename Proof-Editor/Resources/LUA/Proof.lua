local function getAssemblyFiles(directory, is_windows)
	if is_windows then
		handle = io.popen("dir " .. directory .. " /B /A-D")
	end

	t = {}
	for f in handle:lines() do
		if path.hasextension(f, ".dll") then
			if string.find(f, "System.") then
				table.insert(t, f)
			end
		end
	end

	handle:close()
	return t
end

function linkAppReferences()
	local PFDir = os.getenv("PROOF_DIR")
	local monoLibsPath = path.join(PFDir, "Proof-Editor", "mono", "lib", "mono", "4.5")
	local is_windows = os.istarget('windows')

	if is_windows then
		monoLibsPath = monoLibsPath:gsub("/", "\\")
	end

	-- NOTE: We HAVE to use libdirs, using the full path in links won't work
	--				this is a known issue with Visual Studio...
	libdirs { monoLibsPath }

	print("MonoLibsPath: ",monoLibsPath)

	--links { "ProofScriptCore" }

	print("Start Add References: ")
	for k, v in ipairs(getAssemblyFiles(monoLibsPath, is_windows)) do
		print("		Adding reference to: " .. v)
		links { v }
	end
	print("Done AddingReferences")
end
require('..')
local model_class
model_class = treelstm.LSTMSim
local loaded = model_class.load('/home/ubuntu/treelstm/trained_models/rel-bilstm.1l.150d.1.th')

--print(arg[1])
--print(arg[2])

--print(loaded)
local lsent = arg[1]--'My wife is slicing potatoes'
local rsent = arg[2]--'We are going to watch movie tonight'

--print('loading word embeddings')
local emb_dir = '/home/ubuntu/treelstm/data/glove/'
local emb_prefix = emb_dir .. 'glove.840B'
local emb_vocab, emb_vecs = treelstm.read_embedding(emb_prefix .. '.vocab', emb_prefix .. '.300d.th')
local emb_dim = emb_vecs:size(2)

--print(emb_dim)

function tokenize(sent)
	words = {}
	for word in sent:gmatch("%w+") do table.insert(words, word) end
	return words
end

function getVectors(words)
	local count = 0
	for index, word in pairs(words) do
		count = count + 1
		--print(word)	
	end
	local num_unk = 0
	local vecs = torch.Tensor(count, emb_dim)
	local i = 1
	for index, w in pairs(words) do
		if emb_vocab:contains(w) then
    			vecs[i] = emb_vecs[emb_vocab:index(w)]
  		else
    			num_unk = num_unk + 1
    			vecs[i]:uniform(-0.05, 0.05)
  		end
		i = i + 1
	end
	--print(vecs)
	return vecs
end

local lvecs = getVectors(tokenize(lsent))
local rvecs = getVectors(tokenize(rsent))



--print(words)

--local lvecs = torch.Tensor(vocab.size, 300)
--local rvecs = torch.Tensor(vocab.size, 300)
print(loaded:predict(lvecs, rvecs))

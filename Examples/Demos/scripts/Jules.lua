-----------------------------------------------------------------------------------
-- Maratis
-- Jules script test
-----------------------------------------------------------------------------------

-- get objects
Jules = getObject("Jules")
Player = getObject("Player")
Set = getObject("Set")
Feet = getObject("Feet")

-- scene update
function onSceneUpdate()

	coll = getNumCollisions(Feet)

    rotate(Player, {0, 0, 1}, getProperty("YAW"));

	-- rotate left
	if isKeyPressed("LEFT") then
		rotate(Player, {0, 0, 1}, 5)
	end

	-- rotate right
	if isKeyPressed("RIGHT") then
		rotate(Player, {0, 0, 1}, -5)
	end
	
	-- jump (commented as an example)
    
	if isKeyPressed("SPACE") or getTouchPhase(0) == 2 then
		if coll > 1 then
			addCentralForce(Player, {0, 0, 30})
		end
	end
	
	-- move Jules
	if isKeyPressed("UP") then
		changeAnimation(Jules, 2)
		if coll > 1 then
			addCentralForce(Player, {0, 0, 15}, "local")
		end
	else
		changeAnimation(Jules, 0)
	end
	
	-- manual friction (set damping if Player touch the ground)
	if coll < 2 then
		setLinearDamping(Player, 0.01)
	else
		setLinearDamping(Player, 0.9)
	end
	
end